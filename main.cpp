/*
 * Климов А.В. Задача о парковке.
 * alvk.klimov@gmail.com
 * Посчитал необходимыми проккоментировать свое решение.
 * Рассмотрим парковку как систему, на которую оказываются воздействия в виде
 * въезда-выезда машин. Каждое воздействие изменяет состояние системы по параметру
 * кол-во машин. Этот параметр и будем рассматривать. Зафиксировали въезд + 1 машина,
 * зафиксировали выезд - 1 машина.
*/

#include <QCoreApplication>
#include <iostream>
using namespace std;
#include <algorithm>
#include <functional>
#include <iostream>



//тип события: въезд-выезд
enum class ActionType {
    CarIn,
    CarOut
};

//событие происходящее на автостоянке
struct Action {
    Action(const string &dt, ActionType tp) {
        mDt = dt;
        mActionType = tp;
    }

    ActionType actionType() const {
        return mActionType;
    }
    string moment() const {
        return mDt;
    }
private:
    //момент события
    string mDt;
    //его тип, машина въехали или выехала
    ActionType mActionType;
};

//период стоянки машины
struct Period {
    string mStartTime;
    string mEndTime;
};

// кол-во минут из строки вида "чч:мм"
int getTime(const std::string &str) {
    const int MINUTES = 60;
    size_t pos = str.find_first_of(":");
    if (pos != string::npos) {
        string hh = str.substr(0, pos);
        string mm = str.substr(pos+1, str.length() );
        return atoi(hh.c_str()) * MINUTES + atoi(mm.c_str());
    }
    else {
        //для простоты не будем обрабатывать все неккоректные значения
        //или значения когда передаются ошибочные значения
        return -1;
    }

 }
//предикат для сравнения элементов по значению ключа
bool compareMoments(pair<string, int> cur, pair<string, int> next) {
    return cur.second < next.second;
}

//@brief ф-ия поиска максимального кол-ва автомобилей на стоянке
//@param v список пар "въезд-выезд" автомобилей на стоянку
pair<string, int> get_max(const vector<Period> &v) {
    vector<Action> actions;
    //Заполним список событий actions
    for_each(begin(v), end(v), [&actions] (const Period & el){
         actions.push_back( Action(el.mStartTime, ActionType::CarIn) );
         actions.push_back( Action(el.mEndTime, ActionType::CarOut) );
    });

    //отсортируем список событий по времени
    sort( begin(actions), end(actions), [](const Action &first, const Action &second){
        int first_time = getTime(first.moment() );
        int second_time = getTime(second.moment() );

        return first_time < second_time;
    });

    //кол-во машин на парковке в моменты когда туда кто-то въезжает или выезжает
    map<string, int> parkingSystem;

    size_t system_size = 0;
    for_each( begin(actions), end(actions), [&parkingSystem, &system_size](const Action &a){
        string action_tp;
        //машина въехала
        if ( a.actionType() == ActionType::CarIn ) {
            system_size++;
            action_tp = "<--";
            parkingSystem[a.moment()] = system_size;
        }
        else { //машина выехала
            system_size--;
            parkingSystem[a.moment()] = system_size;
            action_tp = "-->";
        }
        cout<<action_tp<<" "<<" "<<a.moment() <<" "<<parkingSystem[a.moment()]<<endl;

    });

    auto max = std::max_element(begin(parkingSystem), end(parkingSystem), compareMoments);
    std::cout<<"Timestamp: "<<(*max).first<< "Number: "<<(*max).second<<endl;
    return *max;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::vector<Period> v = {
        {"11:11", "11:50"},
        {"11:00", "12:00"},
        {"09:20", "23:00"},
        {"09:10", "23:00"},
        {"07:00", "17:00"},
        {"11:11", "12:30"},
        {"19:20", "23:40"},
        {"09:10", "23:00"},
        {"17:00", "17:20"},
    };

    auto max =  get_max(v);
    return a.exec();
}

