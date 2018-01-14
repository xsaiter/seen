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

using str_u = std::wstring;
using vec_str_u = std::vector<str_u>;
using set_str_u = std::set<str_u>;

enum class ops_s { AND, OR };

set_str_u get_stop_words() {
  set_str_u res{L"at", L"me"};
  return res;
}

double get_tf(set_str_u &words, str_u &word) {
  auto n = std::count(words.begin(), words.end(), word);
  return n / words.size();
}

vec_str_u tokenize(const str_u &text) {
  vec_str_u res;

  std::wregex re(LR"(\s+)");

  std::wsregex_token_iterator i(text.begin(), text.end(), re, -1);
  std::wsregex_token_iterator end;

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

void normalize_words(vec_str_u &words) {
  for (auto &word : words) {
    stemming::english_stem<> stem;
    stem(word);
  }
}

struct idx_elt_s {
  long doc_id;
  double tf;
};

using inverted_u = std::map<str_u, std::set<idx_elt_s>>;

double create_tf() { return 1.0; }

std::map<str_u, int> get_words_to_count(const vec_str_u &words) {
  std::map<str_u, int> res;

  for (const str_u &word : words) {
    auto it = res.find(word);
    if (it != res.end()) {
      (*it).second++;
    } else {
      res.insert(std::make_pair(word, 1));
    }
  }

  return res;
}

class idx_s {
public:
  void add_doc(long doc_id, const vec_str_u &words) {
    auto words_to_count = get_words_to_count(words);
    doc_ids_.insert(doc_id);

    auto words_count = words.size();
    auto doc_ids_count = doc_ids_.size();

    for (const str_u &word : words) {
    }
  }

  std::set<long> search_docs(vec_str_u &terms, ops_s op, int limit) const {}

private:
  std::set<long> doc_ids_;
  inverted_u inverted_;
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
  std::wcout << (ok == true ? L"OK-" : L"FAIL-") << message << std::endl;
}

void test_tokenize() {
  auto res = tokenize(L"look at me");
  auto ok = res[0] == L"look" && res[1] == L"at" && res[2] == L"me";
  print_test_result(ok, L"test_tokenize");
}

void test_remove_stop_words() {
  set_str_u stop_words{L"at", L"me"};
  vec_str_u words{L"look", L"at", L"me", L"please"};
  auto res = remove_stop_words(stop_words, words);
  bool ok = res[0] == L"look" && res[1] == L"please";
  print_test_result(ok, L"test_remove_stop_words");
}

void test_stemming_english() {
  std::wstring word(L"transformation");
  stemming::english_stem<> stem;
  stem(word);
  bool ok = L"transform" == word;
  print_test_result(ok, L"test_stemming_english");
}

void test_normalize_words() {
  vec_str_u words{L"transformation", L"collecting"};
  normalize_words(words);
  bool ok = words[0] == L"transform" && words[1] == L"collect";
  print_test_result(ok, L"test_normalize_words");
}

void test_words_to_count() {
  vec_str_u words{L"aa", L"aa", L"bb"};
  auto res = get_words_to_count(words);

  auto it_aa = res.find(L"aa");
  auto it_bb = res.find(L"bb");

  bool ok = false;

  if (it_aa != res.end() && it_bb != res.end()) {
    if ((*it_aa).second == 2 && (*it_bb).second == 1) {
      ok = true;
    }
  }

  print_test_result(ok, L"test_words_to_count");
}

void run_all_tests() {
  test_tokenize();
  test_remove_stop_words();
  test_stemming_english();
  test_normalize_words();
  test_words_to_count();
}

/*
 * main
*/

int main(int argc, char *argv[]) {
  run_all_tests();
  return 0;
}
