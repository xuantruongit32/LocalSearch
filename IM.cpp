// INFLUENCE MAXIMIZATION WITH LIMITED BUDGET
#include<bits/stdc++.h>
using namespace std;
struct KOL {
    string name;
    int numberFollowers;
    vector<int> followerSet;
    double money;
    KOL(string name, int numberFollowers, vector<int> followerSet, double money):name(name), numberFollowers(numberFollowers), followerSet(followerSet), money(money){} // Constructor
    bool operator<(const KOL& other) const {
        return numberFollowers/money < other.numberFollowers/other.money;
}
    bool operator>(const KOL& other) const {
        return numberFollowers/money > other.numberFollowers/other.money;
}
};
bool compareMoney(const KOL &kol1, const KOL &kol2){ return kol1.money < kol2.money;
}


struct Input{ //doc File
    double providedMoney;
    vector<KOL> listKOL;
    string path; 
    Input(string path): path(path){
        readFile();
    }; //constructor
    void readFile(){
        ifstream ifile(path); //Read file data
        string line;
        getline(ifile,line); //Lay dong dau tien
        istringstream iss(line);
        string token;
        iss >> token;
        iss >> token;
        iss >> token;
        providedMoney = stof(token); //Vi providedMoney la tu thu 3 nen getline 3 lan, va phai doi tu string -> float
        getline(ifile,line); //Bo dong thu 2
        while(getline(ifile,line)){
            istringstream iss(line);
            string name;
            iss >> token;
            name = token;
            iss >> token;
            double money = stod(token); //So tien thue KOL
            iss >> token;
            int numberFollowers = stoi(token);
            iss >> token;
            string newToken = token.substr(1, token.length()-2); //Bo dau {} o dau va cuoi
            istringstream iss1(newToken);
            string token2;
            vector<int> followerSet;
            // Cho tat ca cac follwer vao vector<int> followerSet
            while(getline(iss1,token2, ',')){
               followerSet.push_back(stoi(token2)); 
            }
            KOL newKOL(name, numberFollowers, followerSet, money);
            listKOL.push_back(newKOL);
        }
    }

};

struct Solution{
    Input *data;
    double tienConLai;
    unordered_set<int> nguoiTiepCan; //int dau tien la ten nguoi theo doi, int sau la dem so KOL ma nguoi do theo doi
    vector<KOL> KOLChuaThue;
    vector<KOL> KOLDaThue;

    void thueKOL(KOL kol, int i){ //i la vi tri cua kol trong KOLChuaTHue
        tienConLai -= kol.money;
        cout<<tienConLai<<endl;
        for(auto n: kol.followerSet){
            nguoiTiepCan.insert(n);
        }
        KOLChuaThue.erase(KOLChuaThue.begin()+i);
        KOLDaThue.push_back(kol);
    }

    float getScore(){ //Ham danh gia score moi khi da them first KOL
        int score = nguoiTiepCan.size();
        return score;
    }
    float minMoneyKOLChuaThue(){
        auto firstIT = min_element(KOLChuaThue.begin(), KOLChuaThue.end(), compareMoney); 
        return firstIT->money;

    }

    Solution(){}

    Solution(Input &data){ //constructor
        this->data = &data;
        tienConLai = data.providedMoney;
        KOLChuaThue = data.listKOL;
    
    }

    void printSolution(){
        cout<<"Tong tien thue: "<<data->providedMoney-tienConLai<<endl;
        cout<<"So nguoi tiep can: "<<getScore()<<endl;
        cout<<"KOL: "<<endl;
        for(auto c: KOLDaThue){
            cout<<c.name<<endl;
        }
    }
};

Solution greedySolution(Input &data){
    Solution firstSolution(data);
    sort(firstSolution.KOLChuaThue.begin(), firstSolution.KOLChuaThue.end(), greater<KOL>());
            for(int i=0;i<firstSolution.KOLChuaThue.size();i++){
                if(firstSolution.tienConLai<firstSolution.minMoneyKOLChuaThue())
                    break;
                if(firstSolution.tienConLai<firstSolution.KOLChuaThue[i].money)
                    // Neu tien khong du thue KOL nay, bo qua va tiep tuc voi KOL khac
                    continue;
                else
                    firstSolution.thueKOL(firstSolution.KOLChuaThue[i],i);
            }
    return firstSolution;
}
Solution UnGreedySolution(Input &data){
    Solution firstSolution(data);
    sort(firstSolution.KOLChuaThue.begin(), firstSolution.KOLChuaThue.end());
            for(int i=0;i<firstSolution.KOLChuaThue.size();i++){
                if(firstSolution.tienConLai<firstSolution.minMoneyKOLChuaThue())
                    break;
                if(firstSolution.tienConLai<firstSolution.KOLChuaThue[i].money)
                    // Neu tien khong du thue KOL nay, bo qua va tiep tuc voi KOL khac
                    continue;
                else
                    firstSolution.thueKOL(firstSolution.KOLChuaThue[i],i);
            }
    return firstSolution;
}
Solution generateRandomSolution(Input& data) {
    Solution randomSolution(data);
    random_device rd;
    mt19937 generator(rd());
    shuffle(randomSolution.KOLChuaThue.begin(), randomSolution.KOLChuaThue.end(), generator);
    for (int i = 0; i < randomSolution.KOLChuaThue.size(); i++) {
        if(randomSolution.tienConLai<randomSolution.minMoneyKOLChuaThue())
            break;
        if (randomSolution.tienConLai >= randomSolution.KOLChuaThue[i].money) {
        randomSolution.thueKOL(randomSolution.KOLChuaThue[i], i);
    }
    }

    return randomSolution;
}
Solution localSearch(Solution firstSolution) {
    Solution bestSolution = firstSolution;
    bool check = true;
    int count = 0;            
    
    while (check) {
        check = false;
        count++;
        for (int i = 0; i < bestSolution.KOLDaThue.size(); i++) {
            Solution currentSolution = bestSolution;
            
            KOL removedKOL = currentSolution.KOLDaThue[i];
            currentSolution.KOLDaThue.erase(currentSolution.KOLDaThue.begin() + i);
            currentSolution.tienConLai += removedKOL.money;
            
            for (int j = 0; j < currentSolution.KOLChuaThue.size(); j++) {
                KOL addedKOL = currentSolution.KOLChuaThue[j];
                
                if (currentSolution.tienConLai >= addedKOL.money) {
                    currentSolution.thueKOL(addedKOL, j);
                    
                    if (currentSolution.getScore() > bestSolution.getScore()) {
                        bestSolution = currentSolution;
                        count = true;
                    }
                    
                    currentSolution.KOLDaThue.pop_back();
                    currentSolution.KOLChuaThue.insert(currentSolution.KOLChuaThue.begin() + j, addedKOL);
                    currentSolution.tienConLai -= addedKOL.money;
                }
            }
        }
    }
    cout << "So lan chay localsearch" << count << endl;
    return bestSolution;
}

int main (){
    Input io("database/output1.txt");
 //   localSearch(greedySolution(io)).printSolution();
//    localSearch(UnGreedySolution(io)).printSolution();
    localSearch(generateRandomSolution(io)).printSolution();
    }
