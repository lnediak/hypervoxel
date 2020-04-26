#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string dumpFile(const std::string &filename) {
  std::ifstream stream(filename, std::ios_base::binary);
  if (!stream) {
    throw std::runtime_error("Could not open \"" + filename + "\"");
  }
  const int BUF_LEN = 8192;
  char buf[BUF_LEN];
  std::stringstream ss;
  while (stream) {
    stream.read(buf, BUF_LEN);
    ss.write(buf, stream.gcount());
  }
  return ss.str();
}

#define MAX_UINT 1000000000

unsigned int strToUInt(const std::string &str, std::size_t &i) {
  std::size_t len = str.size();
  unsigned int toreturn = 0;
  std::size_t ind = i;
  std::size_t origI = i;
  if (i >= len) {
    ind = origI = 0;
  }
  for (; ind < len; ind++) {
    unsigned char c = str[ind];
    if (!std::isdigit(c)) {
      if (origI == ind || i >= len) {
        throw std::runtime_error(str + " is not a nonnegative integer");
      } else {
        break;
      }
    }
    if (toreturn >= MAX_UINT / 10) {
      throw std::runtime_error(" is out of range");
    }
    toreturn = 10 * toreturn + c - '0';
  }
  i = ind;
  return toreturn;
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
      break;
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

Text nextChunk(const std::string &text, std::size_t &i, bool expandRepeats) {
  std::stringstream ss;
  if (i >= text.size()) {
    return {TextType::end, ""};
  }
  while (true) {
    std::size_t origI = i;
    Text t = nextWord(text, i);
    if (t.type == TextType::end) {
      break;
    }
    if (!t.text.compare(0, 4, "END;")) {
      if (ss.str().size()) {
        i = origI;
      } else {
        ss << t.text;
      }
      break;
    }
    if (t.text.size() >= 6 && !t.text.compare(t.text.size() - 6, 6, "REPEAT")) {
      ss << t.text.substr(0, t.text.size() - 6);
      std::size_t origI = i - 6;
      Text indVar = (nextWord(text, i), nextWord(text, i));
      Text countS = (nextWord(text, i), nextWord(text, i));
      if (countS.type != TextType::text) {
        throw std::runtime_error("Reached EOF during REPEAT statement");
      }
      i -= countS.text.size();
      std::size_t tmp = 0;
      unsigned int count = strToUInt(countS.text, tmp);
      i += tmp;
      std::stringstream repeContent;
      while (true) {
        Text nt = nextChunk(text, i, false);
        if (nt.type == TextType::end) {
          throw std::runtime_error("Reached EOF during REPEAT statement");
        }
        if (nt.text.size() && !nt.text.compare(0, 4, "END;")) {
          i -= nt.text.size() - 4;
          break;
        } else {
          repeContent << nt.text;
        }
      }
      if (expandRepeats) {
        for (unsigned int index = 0; index < count; index++) {
          std::stringstream rand;
          rand << index;
          std::string indexS = rand.str();
          std::string repeC = repeContent.str();
          std::size_t ind = 0;
          while ((ind = repeC.find(indVar.text, ind)) != std::string::npos) {
            repeC.replace(ind, indVar.text.size(), indexS);
            ind += indVar.text.size();
          }
          std::size_t subI = 0;
          while (true) {
            Text nt = nextChunk(repeC, subI, true);
            if (nt.type == TextType::end) {
              break;
            }
            ss << nt.text;
          }
        }
      } else {
        ss << text.substr(origI, i - origI);
      }
      continue;
    } else if (t.text.size() >= 7 &&
               !t.text.compare(t.text.size() - 7, 7, "REPEATB")) {
      ss << t.text.substr(0, t.text.size() - 7);
      std::size_t origI = i - 7;
      Text indVar = (nextWord(text, i), nextWord(text, i));
      Text invVar = (nextWord(text, i), nextWord(text, i));
      Text countS = (nextWord(text, i), nextWord(text, i));
      if (countS.type != TextType::text) {
        throw std::runtime_error("Reached EOF during REPEATB statement");
      }
      i -= countS.text.size();
      std::size_t tmp = 0;
      unsigned int count = strToUInt(countS.text, tmp);
      i += tmp;
      std::stringstream repeContent;
      while (true) {
        Text nt = nextChunk(text, i, false);
        if (nt.type == TextType::end) {
          throw std::runtime_error("Reached EOF during REPEATB statement");
        }
        if (nt.text.size() && !nt.text.compare(0, 4, "END;")) {
          i -= nt.text.size() - 4;
          break;
        } else {
          repeContent << nt.text;
        }
      }
      if (expandRepeats) {
        for (unsigned int index = 0; index < count; index++) {
          std::stringstream rand;
          rand << index;
          std::string indexS = rand.str();
          rand.str("");
          rand << count - 1 - index;
          std::string invS = rand.str();
          std::string repeC = repeContent.str();
          std::size_t ind = 0;
          while ((ind = repeC.find(indVar.text, ind)) != std::string::npos) {
            repeC.replace(ind, indVar.text.size(), indexS);
            ind += indVar.text.size();
          }
          ind = 0;
          while ((ind = repeC.find(invVar.text, ind)) != std::string::npos) {
            repeC.replace(ind, invVar.text.size(), invS);
            ind += invVar.text.size();
          }
          std::size_t subI = 0;
          while (true) {
            Text nt = nextChunk(repeC, subI, true);
            if (nt.type == TextType::end) {
              break;
            }
            ss << nt.text;
          }
        }
      } else {
        ss << text.substr(origI, i - origI);
      }
      continue;
    } else if (t.text.size() >= 7 &&
               !t.text.compare(t.text.size() - 7, 7, "REPEAT2")) {
      ss << t.text.substr(0, t.text.size() - 7);
      std::size_t origI = i - 7;
      Text bitsVar = (nextWord(text, i), nextWord(text, i));
      Text varVar = (nextWord(text, i), nextWord(text, i));
      Text dimsS = (nextWord(text, i), nextWord(text, i));
      if (dimsS.type != TextType::text) {
        throw std::runtime_error("Reached EOF during REPEAT2 statement");
      }
      i -= dimsS.text.size();
      std::size_t tmp = 0;
      unsigned int dims = expandRepeats? strToUInt(dimsS.text, tmp) : (tmp = dimsS.text.size(), 0);
      if (dims >= 24) {
        throw std::runtime_error("count is out of range in REPEAT2 statement");
      }
      i += tmp;
      std::stringstream repeContent;
      while (true) {
        Text nt = nextChunk(text, i, false);
        if (nt.type == TextType::end) {
          throw std::runtime_error("Reached EOF during REPEAT2 statement");
        }
        if (nt.text.size() && !nt.text.compare(0, 4, "END;")) {
          i -= nt.text.size() - 4;
          break;
        } else {
          repeContent << nt.text;
        }
      }
      if (expandRepeats) {
        unsigned int maxBits = (1 << dims);
        std::string indexS(dims, '0');
        for (unsigned int index = 0; index < maxBits; index++) {
          for (unsigned int ii = dims, iii = 0; ii--; iii++) {
            indexS[ii] = ((index & (1 << iii)) >> iii) + '0';
          }
          std::string repeC = repeContent.str();
          std::size_t ind = 0;
          while ((ind = repeC.find(bitsVar.text, ind)) != std::string::npos) {
            repeC.replace(ind, bitsVar.text.size(), indexS);
            ind += bitsVar.text.size();
          }
          std::stringstream expansion;
          std::size_t subI = 0;
          while (true) {
            Text nt = nextChunk(repeC, subI, true);
            if (nt.type == TextType::end) {
              break;
            }
            expansion << nt.text;
          }
          std::string expS = expansion.str();
          std::stringstream rand;
          for (unsigned int ii = dims; ii--;) {
            rand.str("");
            rand << varVar.text << ii;
            ind = 0;
            while ((ind = expS.find(rand.str(), ind)) != std::string::npos) {
              expS.replace(ind, rand.str().size(), std::string(1, indexS[ii]));
              ind += rand.str().size();
            }
          }
          subI = 0;
          while (true) {
            Text nt = nextChunk(expS, subI, true);
            if (nt.type == TextType::end) {
              break;
            }
            ss << nt.text;
          }
        }
      } else {
        ss << text.substr(origI, i - origI);
      }
      continue;
    }
    ss << t.text;
  }
  return {TextType::text, ss.str()};
}

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
      std::size_t i = 0;
      while (true) {
        Text t = nextChunk(tmpT, i, true);
        std::cout << t.text;
        if (t.type == TextType::end) {
          break;
        }
      }
    }
    std::cout << ")OPENCLPP\"" << std::endl;
  } catch (std::runtime_error err) {
    std::cerr << err.what() << std::endl;
    return 1;
  }
}

