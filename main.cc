/*
 * tf - term frequency
 * idf - inverse document frequency
*/

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <hash_map>
#include <regex>
#include <algorithm>

// stemming
#include "stemming/english_stem.h"
#include "stemming/russian_stem.h"

using str_u = std::string;
using vec_str_u = std::vector<str_u>;
using set_str_u = std::set<str_u>;

enum class ops_s { AND, OR };

set_str_u get_stop_words() {
  set_str_u res{"at", "me"};
  return res;
}

double get_tf(set_str_u &words, str_u &word) {
  auto n = std::count(words.begin(), words.end(), word);
  auto m = words.size();
  return n / m;
}

vec_str_u tokenize(const str_u &text) {
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
               [&](const auto &s) {
                 return std::find(stop_words.begin(), stop_words.end(), s) ==
                        stop_words.end();
               });
  return res;
}

void normalize_words(vec_str_u &words) {}

struct idx_elt_s {
  long doc_id;
};

class idx_s {
public:
  void add_doc(long doc_id, const vec_str_u &words) {}

  std::map<str_u, int> words_count(const vec_str_u &words) const {}

  std::set<long> search_docs(vec_str_u &terms, ops_s op, int limit) const {}

private:
  std::set<long> doc_ids_;
  std::map<str_u, std::set<idx_elt_s>> inverted_;
};

class doc_repository_s {
public:
  void add_doc(long id, const str_u &text) {
    auto terms = split_to_terms(text);
    idx_.add_doc(id, terms);
  }

  std::set<long> search_docs(const str_u &query, ops_s op, int limit) {
    auto terms = split_to_terms(query);
    return idx_.search_docs(terms, op, limit);
  }

private:
  idx_s idx_;

  vec_str_u split_to_terms(const str_u &text) {
    auto words = tokenize(text);

    normalize_words(words);

    auto stop_words = get_stop_words();

    return remove_stop_words(stop_words, words);
  }
};

/*
 * tests
*/

void print_test_result(bool ok, const str_u &message) {
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
