#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

// алгоритм раскарски - обход графа в ширину, отмечаем вершины, соединенные более чем с 1 ребром

vector<int> resh(vector< vector<int> >& uz, const vector<int>& ug) {
    vector<int> cveta(uz.size(), -1);
    for(auto it : ug){ int m = it; vector<bool> pokr(uz.size(), 0);
        for(int i = 0; i < uz.size(); i++){
            if(uz[m][i] == 1){ if(cveta[i] > -1)  pokr[cveta[i]] = 1; }
        }
        for(int j = 0; j < uz.size(); j++){
            if(!pokr[j]){ cveta[m] = j; break; }
        }
    }
    return cveta;
}

// заполнение графа из файла

vector <vector <int>> zapoln(const string& ff, vector< pair <int, int> >& spisokuglov, vector<int>& uglinumber) {
    ifstream fn;
    fn.open(ff);
    vector< vector<int> > GR;
    int versh, ugli;
    vector< pair<int, int> > PR;
    spisokuglov.clear();
    uglinumber.clear();
    if(fn.is_open()){ fn >> versh >> ugli; vector <int> v(versh, 0); GR.resize(versh, v); auto zero = make_pair(0, 0);
    PR.resize(GR.size(), zero);
        for(int i = 0; i < ugli; i++){
            int nach, kon;
            fn >> nach >> kon;
            auto k = make_pair(nach, kon); spisokuglov.push_back(k);
            GR[nach][kon] = 1; GR[kon][nach] = 1;
            PR[nach].first++; PR[kon].first++;
            PR[nach].second = nach; PR[kon].second = kon;
        }
    }
    else cout << "Ошибка: не удалось открыть файл" << endl;
    sort(PR.begin(), PR.end());
    for(int j = PR.size() - 1; j >= 0; j--){ uglinumber.push_back(PR[j].second); }
    return GR;
}

// функция main аналогична задаче с рюкзаком, открываем каждый файл с тестами и выполняем их

int main(){
    string path = "data";
    auto it = fs::directory_iterator(path);
    vector<fs::path> array_path;
    copy_if(fs::begin(it), fs::end(it), std::back_inserter(array_path),
        [](const auto& entry){
            return fs::is_regular_file(entry);
        });
    for(auto& p : array_path){
        vector <int> uglinumber;
        vector <pair<int, int>> ugli;
        vector <vector<int>> graph = zapoln(p.string(), ugli, uglinumber);
        vector <int> raskr = resh(graph, uglinumber);
        int max = -1;
        for(int i = 0; i < raskr.size(); i++){ if(raskr[i] > max)  max = raskr[i]; }
        cout << p.string() << " " << max + 1 << endl;
        cout << endl;
        for(auto i : ugli){
            if(raskr[i.first] == raskr[i.second]){
                cout << "Ошибка" << p.string() << endl;
                cout << i.first << " " << i.second << endl;
                break;
            }
        }
    }
    return 0;
}
