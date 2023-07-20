#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>

using namespace std;


/*

*/

// trim from start (in place)
static inline void ltrim(std::string &s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                  { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                       { return !std::isspace(ch); })
              .base(),
          s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
  ltrim(s);
  rtrim(s);
}

struct Exam
{
  public:
  string wm;
  string om;
  vector<string> questions;
  friend ostream &operator<<(ostream &os, const Exam &ex)
  {
    os << ex.wm << "->" << ex.om << '\n';
    for (const auto &q : ex.questions){
      os << "- " << q << "\n";
    }
    return os;
  }
};

void loadExams(vector<Exam> &exams)
{
  ifstream file("stats.md");
  if (!file){
    cerr << "Unable to open file: " << endl;
    return;
  }

  string line;
  Exam current_exam;
  while (getline(file, line))
  {
    if (line.empty()){ // empty line indicates end of current examination
      exams.push_back(current_exam);
      current_exam = Exam(); // reset current_exam
    }
    else if (line.find("->") != string::npos){ // line contains wm and om
      istringstream ss(line);
      getline(ss, current_exam.wm, '-'); // discard '-'
      getline(ss, current_exam.wm, '-'); // read up to '-'
      getline(ss, current_exam.om, '>'); // discard '>'
      getline(ss, current_exam.om);      // read the rest of the line

      trim(current_exam.wm);
      trim(current_exam.om);
    }
    else{ // line contains a question
      istringstream ss(line);
      string new_question;
      getline(ss, new_question, '-'); // discard '-
      getline(ss, new_question);
      trim(new_question);
      current_exam.questions.push_back(new_question);
    }
  }
  // push back the last examination if the file doesn't end with an empty line
  if (!current_exam.questions.empty()){
    exams.push_back(current_exam);
  }
}

int main(int argc, char *argv[]){
  ofstream out("output.txt");
  vector<Exam> exams;
  loadExams(exams);
  /*for (auto &ex : stats)
    cout << ex;
  */
  // exams to stats
  // question, wm, ?
  map<string, pair<list<string>, int>> stats;
  for (auto &ex : exams){
    for(auto q : ex.questions){
      if(stats.find(q) == stats.end()){
        stats.insert({q, {list<string>(), 0}});
      }
      if(ex.wm.find("?") == string::npos)
        stats[q].first.push_back(ex.wm);
      else
        stats[q].second += 1;
    }
  }

  /*
  for(auto &a: stats){
    cout << a.first << ' ' << a.second.second << endl;
    for(auto m: a.second.first)
      cout << m << ' ';
    cout << endl;
  }*/
  // work on stats
  // ranges: 18-22, 23-26, 27+
  // question, ranges, tot
  map<string, pair<vector<int>, int>> wstats;
  for(auto &q : stats){
    if(wstats.find(q.first) == wstats.end()){
      wstats.insert({q.first, {vector<int>(3, 0), q.second.second}});
    }
    for(auto& wm :q.second.first){
      wstats[q.first].second++;
      if(wm <= "22"){
        wstats[q.first].first[0]++;
      }
      else if (wm <= "26"){
        wstats[q.first].first[1]++;
      }
      else
      {
        wstats[q.first].first[2]++;
      }
    }
  }
  for(auto &q : wstats){
    cout << q.first << ' ' << q.second.second << endl;
    for (auto m : q.second.first)
      cout << m << ' ';
    cout << endl;
  }
  out.close();
  return 0;
}
