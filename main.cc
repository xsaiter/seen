#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <hash_map>
#include <regex>

using words_u = std::vector<std::string>;

std::vector<std::string> tokenize(const std::string &text) {}

words_u normalize_words(const words_u &words) {}

words_u remove_stop_words(const std::set<std::string> &stop_words,
                          const words_u &words) {}

class indexer_s {
public:
  void add_doc(long doc_id, const words_u &words) {}

  std::map<std::string, int> words_count(const words_u &words) {}

  class item_s {};

private:
  std::set<long> doc_ids_;
  std::map<std::string, std::set<item_s>> inverted_;
};

class doc_repository_s {
public:
  void add(long doc_id, const std::string &doc_text) {}

  void search(const std::string &query, int limit) {}

private:
  indexer_s indexer_;
};

int main(int argc, char *argv[]) { return 0; }
