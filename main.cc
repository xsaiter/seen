#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <hash_map>
#include <regex>
#include <algorithm>

using words_u = std::vector<std::string>;

std::vector<std::string> tokenize(const std::string &text) {
  std::vector<std::string> res;

  std::regex re(R"(\s+)");

  std::sregex_token_iterator i(text.begin(), text.end(), re, -1);
  std::sregex_token_iterator end;

  while (i != end) {
    if ((*i).length() > 1) {
      res.push_back(*i);
    }
    ++i;
  }

  return res;
}

words_u remove_stop_words(const std::set<std::string> &stop_words,
                          const words_u &words) {
  words_u res;
  std::copy_if(words.begin(), words.end(), std::back_inserter(res),
               [&](const std::string &s) {
                 return std::find(stop_words.begin(), stop_words.end(), s) ==
                        stop_words.end();
               });
  return res;
}

void normalize_words(words_u &words) {}

std::vector<std::string> get_stop_words() {
  std::vector<std::string> res{"at", "me"};
  return res;
}

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
  void add_doc(long id, const std::string &text) {
    auto words = tokenize(text);

    normalize_words(words);

    auto stop_words = get_stop_words();

    words = remove_stop_words(stop_words, words);

    indexer_.add_doc(id, words);
  }

  std::vector<long> search_docs(const std::string &query, int limit) {}

private:
  indexer_s indexer_;
};

/*
 * tests
*/

void print_test_result(bool ok, const std::string &message) {
  std::cout << (ok == true ? "OK-" : "FAIL-") << message << std::endl;
}

void test_tokenize() {
  auto res = tokenize("look at me");
  auto ok = res[0] == "look" && res[1] == "at" && res[2] == "me";
  print_test_result(ok, "test_tokenize");
}

void test_remove_stop_words() {
  std::set<std::string> stop_words{"at", "me"};
  std::vector<std::string> words{"look", "at", "me", "please"};
  auto res = remove_stop_words(stop_words, words);
  bool ok = res[0] == "look" && res[1] == "please";
  print_test_result(ok, "test_remove_stop_words");
}

void run_all_tests() {
  test_tokenize();
  test_remove_stop_words();
}

/*
 * main
*/

int main(int argc, char *argv[]) {
  run_all_tests();
  return 0;
}
