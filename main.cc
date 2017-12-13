#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <hash_map>
#include <regex>
#include <algorithm>

#include "stemming/english_stem.h"
#include "stemming/russian_stem.h"

using vec_str_u = std::vector<std::string>;
using set_str_u = std::set<std::string>;

enum class ops_s { AND, OR };

vec_str_u tokenize(const std::string &text) {
  vec_str_u res;

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

vec_str_u remove_stop_words(const set_str_u &stop_words,
                            const vec_str_u &words) {
  vec_str_u res;
  std::copy_if(words.begin(), words.end(), std::back_inserter(res),
               [&](const std::string &s) {
                 return std::find(stop_words.begin(), stop_words.end(), s) ==
                        stop_words.end();
               });
  return res;
}

void normalize_words(vec_str_u &words) {}

set_str_u get_stop_words() {
  set_str_u res{"at", "me"};
  return res;
}

class indexer_s {
public:
  void add_doc(long doc_id, const vec_str_u &words) {}

  std::map<std::string, int> words_count(const vec_str_u &words) {}

  std::vector<long> search_docs(vec_str_u &terms, ops_s op, int limit) {}

  class item_s {};

private:
  std::set<long> doc_ids_;
  std::map<std::string, std::set<item_s>> inverted_;
};

class doc_repository_s {
public:
  void add_doc(long id, const std::string &text) {
    auto terms = get_terms(text);
    indexer_.add_doc(id, terms);
  }

  std::vector<long> search_docs(const std::string &query, ops_s op, int limit) {
    auto terms = get_terms(query);
    return indexer_.search_docs(terms, op, limit);
  }

private:
  indexer_s indexer_;

  vec_str_u get_terms(const std::string &text) {
    auto words = tokenize(text);

    normalize_words(words);

    auto stop_words = get_stop_words();

    return remove_stop_words(stop_words, words);
  }
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
  set_str_u stop_words{"at", "me"};
  vec_str_u words{"look", "at", "me", "please"};
  auto res = remove_stop_words(stop_words, words);
  bool ok = res[0] == "look" && res[1] == "please";
  print_test_result(ok, "test_remove_stop_words");
}

void test_stemming_english() {
  std::wstring word(L"transformation");
  stemming::english_stem<> stem;
  stem(word);
  bool ok = L"transform" == word;
  print_test_result(ok, "test_stemming_english");
}

void run_all_tests() {
  test_tokenize();
  test_remove_stop_words();
  test_stemming_english();
}

/*
 * main
*/

int main(int argc, char *argv[]) {
  run_all_tests();
  return 0;
}
