#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

std::string dumpStream(std::istream &stream) {
  const int BUF_LEN = 8192;
  char buf[BUF_LEN];
  std::stringstream ss;
  while (stream) {
    stream.read(buf, BUF_LEN);
    ss.write(buf, stream.gcount());
  }
  return ss.str();
}

std::string dumpFile(const std::string &filename) {
  if (filename == "-") {
    return dumpStream(std::cin);
  }
  std::ifstream stream(filename, std::ios_base::binary);
  if (!stream) {
    throw std::runtime_error("Could not open \"" + filename + "\"");
  }
  return dumpStream(stream);
}

#define MAX_UINT 1000000000

unsigned int strToUInt(const std::string &str, std::size_t &i) {
  std::size_t len = str.size();
  unsigned int toreturn = 0;
  bool found = false;
  std::size_t ind = i;
  std::size_t origI = i;
  if (i >= len) {
    ind = origI = 0;
  }
  for (; ind < len; ind++) {
    unsigned char c = str[ind];
    if (!std::isdigit(c)) {
      if (!found && std::isspace(c)) {
        continue;
      }
      if (i >= len) {
        throw std::runtime_error(str + " is not a nonnegative integer");
      } else {
        break;
      }
    }
    if (toreturn >= MAX_UINT / 10) {
      throw std::runtime_error(str + " is out of range");
    }
    found = true;
    toreturn = 10 * toreturn + c - '0';
  }
  i = ind;
  return toreturn;
}

std::string trim(const std::string &input) {
  std::size_t i = 0, len = input.size();
  for (; i < len; i++) {
    unsigned char c = input[i];
    if (!std::isspace(c)) {
      break;
    }
  }
  if (i == len) {
    return "";
  }
  std::size_t j = input.size();
  while (j--) {
    unsigned char c = input[j];
    if (!std::isspace(c)) {
      break;
    }
  }
  return input.substr(i, j + 1 - i);
}

enum class TextType { text, whitespace, end };

struct Text {

  TextType type;
  std::string text;
};

Text nextWord(const std::string &text, std::size_t &i) {
  std::size_t len = text.size();
  Text toreturn{TextType::end, ""};
  if (i >= len) {
    return toreturn;
  }
  std::size_t origI = i;
  while (i < len) {
    unsigned char c = text[i];
    bool isW = std::isspace(c);
    if (toreturn.type == TextType::text) {
      if (isW) {
        break;
      }
    } else if (toreturn.type == TextType::whitespace) {
      if (!isW) {
        break;
      }
    } else {
      if (isW) {
        toreturn.type = TextType::whitespace;
      } else {
        toreturn.type = TextType::text;
      }
    }
    i++;
  }
  toreturn.text = text.substr(origI, i - origI);
  return toreturn;
}

class Symbol;

typedef std::vector<std::unique_ptr<Symbol>> SymbolList;

class Symbol {

public:
  virtual std::string getName() const = 0;
  virtual std::string process(const std::string &input,
                              SymbolList &symbols) const = 0;

  virtual ~Symbol() {}
};

std::string expand(const std::string &text, std::size_t i,
                   SymbolList &symbols) {
  if (i >= text.size()) {
    return "";
  }
  struct StackEntry {
    std::size_t i, ii;
    std::string sym;
    std::stringstream ss;
  };
  std::vector<StackEntry> stack;
  stack.push_back({0, std::string::npos, "", std::stringstream()});
  std::size_t quoteDepth = 0;
  while (true) {
    std::cout << std::endl << std::endl << "ENTRY: ----" << std::endl;
    for (const auto &e : stack) {
      std::cout << e.i << " " << e.ii << " " << e.sym << " " << e.ss.str() << std::endl;
    }
    std::size_t origI = i;
    Text t = nextWord(text, i);
    if (t.type == TextType::end) {
      break;
    }
    if (t.type == TextType::whitespace) {
      stack.back().ss << t.text;
      continue;
    }
    if (stack.size() > 1 && !t.text.compare(0, 4, "END;")) {
      i -= t.text.size();
      if (stack.back().ii == std::string::npos) {
        if (!quoteDepth) {
          stack[stack.size() - 2].ss << expand(stack.back().ss.str(), 0, symbols);
        }
      } else if (stack.back().ii == std::string::npos - 1) {
        if (!quoteDepth) {
          stack[stack.size() - 2].ss
              << trim(expand(stack.back().ss.str(), 0, symbols));
        }
      } else if (stack.back().ii == std::string::npos - 2) {
        quoteDepth--;
        stack[stack.size() - 2].ss
            << text.substr(stack.back().i, i - stack.back().i);
      } else if (stack.back().ii == std::string::npos - 3) {
        quoteDepth--;
        stack[stack.size() - 2].ss
            << trim(text.substr(stack.back().i, i - stack.back().i));
      } else if (stack.size() == 2) {
          stack[0].ss << symbols[stack[1].ii]->process(stack[1].ss.str(),
                                                       symbols);
      } else {
        if (!quoteDepth) {
          stack[stack.size() - 2].ss << stack.back().sym << stack.back().ss.str()
                                     << "END;";
        }
      }
      stack.pop_back();
      i += 4;
      continue;
    }
    bool added = false;
    if (t.type == TextType::text) {
      if (t.text.size() >= 6 &&
          !t.text.compare(t.text.size() - 6, 6, "EXPAND")) {
        stack.back().ss << text.substr(origI, t.text.size() - 6);
        added = true;
        stack.push_back({i, std::string::npos, "EXPAND", std::stringstream()});
      } else if (t.text.size() >= 7 &&
                 !t.text.compare(t.text.size() - 7, 7, "EXPANDT")) {
        stack.back().ss << text.substr(origI, t.text.size() - 7);
        added = true;
        stack.push_back(
            {i, std::string::npos - 1, "EXPANDT", std::stringstream()});
      } else if (t.text.size() >= 5 &&
                 !t.text.compare(t.text.size() - 5, 5, "QUOTE")) {
        quoteDepth++;
        stack.back().ss << text.substr(origI, t.text.size() - 5);
        added = true;
        stack.push_back(
            {i, std::string::npos - 2, "QUOTE", std::stringstream()});
      } else if (t.text.size() >= 6 &&
                 !t.text.compare(t.text.size() - 6, 6, "QUOTET")) {
        quoteDepth++;
        stack.back().ss << text.substr(origI, t.text.size() - 6);
        added = true;
        stack.push_back(
            {i, std::string::npos - 3, "QUOTET", std::stringstream()});
      } else {
        for (std::size_t ii = 0, len = symbols.size(); ii < len; ii++) {
          std::string symName = symbols[ii]->getName();
          if (t.text.size() >= symName.size() &&
              !t.text.compare(t.text.size() - symName.size(), symName.size(),
                              symName)) {
            stack.back().ss
                << text.substr(origI, t.text.size() - symName.size());
            added = true;
            stack.push_back({i, ii, symName, std::stringstream()});
            break;
          }
        }
      }
    }
    if (added) {
      continue;
    }
    stack.back().ss << t.text;
  }
  if (stack.size() > 1) {
    throw std::runtime_error("Reached EOF during statement");
  }
  return stack[0].ss.str();
}

class CustomSymbol : public Symbol {

public:
  struct Param {
    std::string name;
    bool isNum;
  };

  CustomSymbol(const std::string &name, const std::vector<Param> &params,
               const std::string &content)
      : name(name), params(params), content(content) {}

  std::string getName() const override { return name; }

  std::string process(const std::string &input,
                      SymbolList &symbols) const override {
    std::size_t i = 0;
    std::vector<std::pair<std::string, std::string>> replacements;
    Text paramText;
    for (const Param &param : params) {
      paramText = nextWord(input, i);
      if (paramText.type != TextType::text) {
        paramText = nextWord(input, i);
      }
      if (paramText.type != TextType::text) {
        throw std::runtime_error("Reached EOF during statement");
      }
      std::string val;
      if (param.isNum) {
        i -= paramText.text.size();
        std::size_t tmp = 0;
        strToUInt(paramText.text, tmp);
        val = paramText.text.substr(0, tmp);
        i += tmp;
      } else {
        val = paramText.text;
      }
      replacements.emplace_back(param.name, val);
    }
    std::string toreturn = content;
    std::string restcontent = input.substr(i);
    i = 0;
    while ((i = toreturn.find("......", i)) != std::string::npos) {
      toreturn.replace(i, 6, restcontent);
      i += restcontent.size();
    }
    for (const auto &e : replacements) {
      i = 0;
      while ((i = toreturn.find(e.first, i)) != std::string::npos) {
        toreturn.replace(i, e.first.size(), e.second);
        i += e.second.size();
      }
    }
    return expand(toreturn, 0, symbols);
  }

private:
  std::string name;
  std::vector<Param> params;
  std::string content;
};

class DefineSymbol : public Symbol {

public:
  std::string getName() const override { return "DEFINE"; }

  std::string process(const std::string &input,
                      SymbolList &symbols) const override {
    std::size_t i = 0;
    Text symName = nextWord(input, i);
    if (symName.type != TextType::text) {
      symName = nextWord(input, i);
    }
    if (symName.type != TextType::text) {
      throw std::runtime_error("Reached EOF during DEFINE statement");
    }
    if (symName.text.size() < 2 || symName.text.back() != 't' || symName.text[symName.text.size() - 2] != 'e') {
      throw std::runtime_error("Invalid syntax for DEFINE statement");
    }
    std::string symtxt = symName.text.substr(0, symName.text.size() - 2);
    Text numParamsS = nextWord(input, i);
    if (numParamsS.type != TextType::text) {
      numParamsS = nextWord(input, i);
    }
    if (numParamsS.type != TextType::text) {
      throw std::runtime_error("Reached EOF during DEFINE statement");
    }
    i -= numParamsS.text.size();
    std::size_t tmp = 0;
    unsigned int numParams = strToUInt(numParamsS.text, tmp);
    i += tmp;
    std::vector<CustomSymbol::Param> params(numParams);
    Text currParam;
    for (std::size_t pi = 0; pi < numParams; pi++) {
      currParam = nextWord(input, i);
      if (currParam.type != TextType::text) {
        currParam = nextWord(input, i);
      }
      if (currParam.type != TextType::text || currParam.text.empty()) {
        throw std::runtime_error(
            "Reached EOF during DEFINE statement while parsing params");
      }
      if (currParam.text[0] == '~') {
        params[pi] = {currParam.text.substr(1), true};
      } else {
        params[pi] = {currParam.text, false};
      }
    }
    for (auto iter = symbols.begin(), iter_end = symbols.end();
         iter != iter_end; ++iter) {
      if (iter->get()->getName() == symtxt) {
        iter->reset(new CustomSymbol(symtxt, params, input.substr(i)));
        return "";
      }
    }
    symbols.emplace_back(
        new CustomSymbol(symtxt, params, input.substr(i)));
    return "";
  }
};

class TrimSymbol : public Symbol {

public:
  std::string getName() const override { return "TRIM"; }

  std::string process(const std::string &input, SymbolList &) const override {
    return trim(input);
  }
};

class WhileSymbol : public Symbol {

public:
  std::string getName() const override { return "WHILE"; }

  std::string process(const std::string &input,
                      SymbolList &symbols) const override {
    std::size_t i = 0;
    Text var = nextWord(input, i);
    if (var.type != TextType::text) {
      var = nextWord(input, i);
    }
    if (var.type != TextType::text) {
      throw std::runtime_error("Reached EOF during WHILE statement");
    }
    if (var.text.size() < 2 || var.text.back() != 't' || var.text[var.text.size() - 2] != 'e') {
      throw std::runtime_error("Invalid syntax for WHILE statement");
    }
    std::string vartxt = var.text.substr(0, var.text.size() - 2);
    std::string subinput = input.substr(i);
    std::stringstream ss;
    while (true) {
      bool found = false;
      bool shouldBreak = false;
      for (const auto &e : symbols) {
        if (e->getName() == vartxt) {
          found = true;
          std::string res = trim(e->process("", symbols));
          shouldBreak = res == "" || res == "0";
          break;
        }
      }
      if (!found || shouldBreak) {
        break;
      }
      ss << expand(subinput, 0, symbols);
    }
    return ss.str();
  }
};

class SumSymbol : public Symbol {

public:
  std::string getName() const override { return "SUM"; }

  std::string process(const std::string &input, SymbolList &) const override {
    unsigned int total = 0;
    std::size_t i = 0;
    while (true) {
      Text val = nextWord(input, i);
      if (val.type != TextType::text) {
        val = nextWord(input, i);
      }
      if (val.type != TextType::text) {
        break;
      }
      std::size_t tmp = std::string::npos;
      total += strToUInt(val.text, tmp);
      if (total >= MAX_UINT) {
        throw std::runtime_error("SUM is out of range");
      }
    }
    std::stringstream ss;
    ss << total;
    return ss.str();
  }
};

class DiffSymbol : public Symbol {

public:
  std::string getName() const override { return "DIFF"; }

  std::string process(const std::string &input, SymbolList &) const override {
    std::size_t i = 0;
    Text a = nextWord(input, i);
    if (a.type != TextType::text) {
      a = nextWord(input, i);
    }
    Text b = nextWord(input, i);
    if (b.type != TextType::text) {
      b = nextWord(input, i);
    }
    if (b.type != TextType::text) {
      throw std::runtime_error("Reached EOF during DIFF statement");
    }
    std::size_t tmp = std::string::npos;
    unsigned int first = strToUInt(a.text, tmp);
    tmp = std::string::npos;
    unsigned int second = strToUInt(b.text, tmp);
    if (first < second) {
      throw std::runtime_error("DIFF cannot produce negative numbers");
    }
    std::stringstream ss;
    ss << first - second;
    return ss.str();
  }
};

class ReplaceSymbol : public Symbol {

public:
  std::string getName() const override { return "REPLACE"; }

  std::string process(const std::string &input,
                      SymbolList &symbols) const override {
    std::size_t i = 0;
    Text a = nextWord(input, i);
    if (a.type != TextType::text) {
      a = nextWord(input, i);
    }
    Text b = nextWord(input, i);
    if (b.type != TextType::text) {
      b = nextWord(input, i);
    }
    if (b.type != TextType::text) {
      throw std::runtime_error("Reached EOF during REPLACE statement");
    }
    if (b.text == "EMPTYSTRING") {
      b.text = "";
    }
    std::string toreturn = input.substr(i);
    i = 0;
    while ((i = toreturn.find(a.text, i)) != std::string::npos) {
      toreturn.replace(i, a.text.size(), b.text);
      i += b.text.size();
    }
    return expand(toreturn, 0, symbols);
  }
};

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout << "Usage: " << (argc ? argv[0] : "preprocess")
              << " filename min_dim max_dim" << std::endl;
    return 0;
  }
  std::size_t i = std::string::npos;
  std::size_t min_dim = strToUInt(argv[2], i);
  i = std::string::npos;
  std::size_t max_dim = strToUInt(argv[3], i);
  try {
    std::string text = dumpFile(argv[1]);
    std::size_t subI = 0;
    SymbolList symbols;
    symbols.emplace_back(new DefineSymbol());
    symbols.emplace_back(new WhileSymbol());
    symbols.emplace_back(new SumSymbol());
    symbols.emplace_back(new DiffSymbol());
    symbols.emplace_back(new ReplaceSymbol());
    symbols.emplace_back(new CustomSymbol(
        "REPEATB", {{"IND", false}, {"INV", false}, {"DIMS", true}},
        // clang-format off
        "DEFINE DECRIIet 0 DIMS END;"
        "DEFINE INCRIIet 0 0 END;"
        "WHILE DECRIIet "
          "QUOTE "
            "DEFINE DECRIIet 0 "
              "EXPAND "
                "DIFF "
                  "EXPAND "
                    "DECRII END;"
                  "END;"
                  " 1 "
                "END;"
              "END; "
            "END;"
            "REPLACE IND "
              "EXPAND "
                "INCRII END;"
              "END;"
              "EXPAND "
                "REPLACE INV "
                  "EXPAND "
                   "DECRII END;"
                  "END;"
                  " ...... "
                "END;"
              "END;"
            "END;"
            "DEFINE INCRIIet 0 "
              "EXPAND "
                "SUM "
                  "EXPAND "
                    "INCRII END;"
                  "END;"
                  " 1 "
                "END;"
              "END; "
            "END;"
          "END;"
        "END;"));
    // clang-format on
    symbols.emplace_back(
        new CustomSymbol("REPEAT", {{"IND", false}, {"DIMS", true}},
                         "REPEATB IND INVPLACEHOLDER DIMS ...... END;"));
    symbols.emplace_back(new CustomSymbol(
        "REPEAT2", {{"BITS", false}, {"VARPRE", false}, {"DIMS", true}},
        // clang-format off
        "DEFINE RP2CONDDIMSet 0 DIMS END;"
        "DEFINE RP2COND2DIMSet 0 1 END;"
        "WHILE RP2CONDDIMSet "
          "DEFINE RP2CONDDIMSet 0 0 END;"
          "DEFINE RP2COND2DIMSet 0 0 END;"
          "QUOTE "
            "REPEAT2 BITS VARPRE "
              "EXPAND "
                "DIFF DIMS 1 END;"
              "END;"
              "EXPAND "
                "REPLACE BITS BITS0 "
                  "EXPAND "
                    "REPLACE "
                      "VARPRE" "EXPANDT "
                                 "DIFF DIMS 1 END;"
                               "END; "
                      "0 "
                      "...... "
                    "END;"
                  "END;"
                "END;"
              "END;"
            "END;"
            "REPEAT2 BITS VARPRE "
              "EXPAND "
                "DIFF DIMS 1 END;"
              "END;"
              "EXPAND "
                "REPLACE BITS BITS1 "
                  "EXPAND "
                    "REPLACE "
                      "VARPRE" "EXPANDT "
                                 "DIFF DIMS 1 END;"
                               "END;"
                      "1 "
                      "...... "
                    "END;"
                  "END;"
                "END;"
              "END;"
            "END;"
          "END;"
        "END;"
        "WHILE RP2COND2DIMSet "
          "REPLACE BITS EMPTYSTRING ...... END;"
          "DEFINE RP2COND2DIMSet 0 0 END;"
        "END;"));
    // clang-format on
    while (true) {
      Text t = nextWord(text, subI);
      if (t.type == TextType::end) {
        break;
      }
    }
    std::cout << "// AUTO-GENERATED FILE: DO NOT MODIFY" << std::endl
              << "R\"OPENCLPP(" << std::endl;
    for (std::size_t numDims = min_dim; numDims <= max_dim; numDims++) {
      std::stringstream rand;
      rand << numDims;
      std::string numDimsS = rand.str();
      std::string tmpT = text;
      std::size_t ind = 0;
      while ((ind = tmpT.find("NUMDIMS", ind)) != std::string::npos) {
        tmpT.replace(ind, 7, numDimsS);
        ind += 7;
      }
      std::cout << expand(tmpT, 0, symbols);
    }
    std::cout << ")OPENCLPP\"" << std::endl;
  } catch (std::runtime_error err) {
    std::cerr << err.what() << std::endl;
    return 1;
  }
}

