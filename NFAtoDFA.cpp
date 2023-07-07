#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
using namespace std;

//存储NFA中一个状态在接受输入后的次态
typedef struct
{
    char state;
    vector<string> move;
}NFAmove;

//存储DFA中一个状态在接受输入后的次态
typedef struct
{
    string state;
    vector<string> move;
}DFAmove;

//分别用于NFA和DFA状态转移表排序的比较函数
bool NFAcmp(NFAmove a, NFAmove b);
bool DFAcmp(DFAmove a, DFAmove b);

//NFA状态转移表类
class NFAtable
{
    public:
        NFAtable();
        //构造函数，s为状态集，默认一个非空字符为一个状态。in是输入集，默认一个非空字符为一个输入。
        //sNo是状态集大小，iNo是输入集大小
        NFAtable(const string& s, const string& in, int sNo, int iNo);  
        ~NFAtable();

        int searchState(char s) const;  //返回状态s在状态转移表的行序号，没有则返回-1
        void sortTable();               //为状态转移表的行做排序
        void inputMove();               //接受用户输入状态对每个输入的次态
        void outputTable() const;       //格式化输出状态转移表

    public:
        vector<char> input;     //输入集，默认一个非空字符为一个输入
        vector<NFAmove> table;  //状态转移表，默认一个非空字符为一个状态，初态在首行
        int stateNo;            //状态数量
        int inputNo;            //输入集大小
};

NFAtable::NFAtable()
    : stateNo(0)
    , inputNo(0)
{
    //默认构造函数将NFA状态转移表设为空
    input.clear();
    table.clear();
};

//构造函数，s为状态集，默认一个非空字符为一个状态。in是输入集，默认一个非空字符为一个输入状态。
//sNo是状态集大小，iNo是输入集大小
NFAtable::NFAtable(const string& s, const string& in, int sNo, int iNo)
    : stateNo(sNo)
    , inputNo(iNo)
{
    NFAmove m;
    m.move.clear();
    input.clear();
    table.clear();
    for(int i = 0; i < inputNo; i++)
    {
        input.push_back(in[i]); //设置输入集
    }

    for (int i = 0; i < stateNo; i++)
    {
        //利用状态集建立状态转移表
        m.state = s[i];         
        table.push_back(m);
    }
}

NFAtable::~NFAtable()
{};

//返回状态s在状态转移表的行序号，没有则返回
int NFAtable::searchState(char s) const
{
    int found = -1;

    //找到状态则立即返回行号
    for (int i = 0; i < stateNo && found == -1; i++)
    {
        if (s == table[i].state)
            found = i;
    } 

    return found;
}

//为状态转移表的行做排序
void NFAtable::sortTable()
{
    sort(table.begin(), table.end(), NFAcmp);
}

//接受用户输入状态对每个输入的转移
void NFAtable::inputMove()
{
    string m;
    for (int i = 0; i < stateNo; i++)
    {
        for(int j = 0; j < inputNo; j++)
        {
            //输入提示
            cout << "状态" << table[i].state << "输入" << input[j] << ':' << endl;
            getline(cin, m);            //接受输入，以换行结束。如果没有转移则直接换行。
            sort (m.begin(), m.end());  //给次态排序
            table[i].move.push_back(m); //添加到转移表
        }
    }
    sortTable();    //给转移表排序
}

//格式化输出状态转移表
void NFAtable::outputTable() const
{
    int i, j;

    cout << setw(10) << ' ';
    for (i = 0; i < inputNo; i++)
    {
        cout << right << setw(10) << input[i]; 
    }
    cout << endl;

    for (i = 0; i < stateNo; i++)
    {
        cout << left << setw(10) << table[i].state;
        for (j = 0; j < inputNo; j++)
        {
            cout << right << setw(10) << ((table[i].move[j].empty()) ? "NULL" : table[i].move[j]); //转移的下一态集合为空则以NULL表示
        }
        cout << endl;
    }
}

//DFA状态转移表类
class DFAtable
{
    public:
        DFAtable();
        ~DFAtable();

        void addFromNFA(const NFAtable& nfa);       //从NFA nfa中添加状态
        int searchState(const string& s) const;     //返回状态s在转移表的行号，没有则返回-1
        void outputTable() const;                   //格式化输出DFA状态转移表   

    private:
        void addState(const string& s, const NFAtable& nfa);    //将新状态s添加进DFA的状态转移表，从参数nfa获取次态
        void sortTable();                                       //为状态转移表排序
        void checkNewState(int s, const NFAtable& nfa);         //检查状态转移表中序号为s的行是否存在新状态

    public:
        vector<char> input;     //输入集，默认一个非空字符为一个输入
        vector<DFAmove> table;  //状态转移表，一个非空string为一个状态，初态在首行
        int stateNo;            //状态数量 
        int inputNo;            //输入集大小
};

DFAtable::DFAtable()
    : stateNo(0)
    , inputNo(0)
{
    //默认构造函数将DFA状态转移表设为空
    input.clear();
    table.clear();
};

DFAtable::~DFAtable()
{};

//从NFA nfa中添加状态
void DFAtable::addFromNFA(const NFAtable& nfa)
{
    //输入集与nfa的一致
    input = nfa.input;
    inputNo = nfa.inputNo;

    //从初态开始添加
    //只添加从初态可达的状态
    stateNo = 1;    //状态数为1
    DFAmove m;
    string s(1, nfa.table[0].state);    //获取初态
    vector<string> n(nfa.table[0].move);//获取初态对每个输入的次态
    m.move = n;
    m.state = s;
    table.push_back(m);                 //加入转移表的首行

    for (int i = 0; i < inputNo; i++)
    {
        //只有当初态的次态非空且未被添加时，才加入
        if (table[0].move[i].size() && searchState(table[0].move[i]) == -1)
                addState(table[0].move[i], nfa);
    }

    //排序状态转移表
    sortTable();
}

//返回状态s在转移表的行号，没有则返回-1
int DFAtable::searchState(const string& s) const
{
    int found = -1;

    //找到状态则立即返回行号
    for (int i = 0; i != table.size() && found == -1; i++)
    {
        if (!s.compare(table[i].state))
            found = i;
    }

    return found;
}

//格式化输出DFA状态转移表
void DFAtable::outputTable() const
{
    int i, j;

    cout << setw(10) << ' ';
    for (i = 0; i < inputNo; i++)
    {
        cout << right << setw(10) << input[i]; 
    }
    cout << endl;

    for (i = 0; i < stateNo; i++)
    {
        cout << left << setw(10) << table[i].state;
        for (j = 0; j < inputNo; j++)
        {
            cout << right << setw(10) << ((table[i].move[j].empty()) ? "NULL" : table[i].move[j]);  //转移的下一态集合为空则以NULL表示
        }
        cout << endl;
    }
}

//将新状态s添加进DFA的状态转移表，从参数nfa获取下一态
void DFAtable::addState(const string& s, const NFAtable& nfa)
{
    int len = s.size();   //新状态的长度
    DFAmove newRow;

    newRow.state = s;
    stateNo++;              //状态数加一

    for (int i = 0; i < inputNo; i++)
    {
        string next;    //新状态对当前序号为i的输入的次态
        for (int j = 0; j < len; j++)
        {
            int row = nfa.searchState(s[j]);                //获取新状态中，单个状态（字符）在NFA的行序号
            string singleStateNext(nfa.table[row].move[i]); //该单个状态对每个输入的次态
            for (int k = 0; k < singleStateNext.length(); k++)
            {
                //如果次态未被添加，则加入next
                if (next.find(singleStateNext[k]) == -1)
                    next.push_back(singleStateNext[k]);
                else if (next.find(singleStateNext) != -1)
                    break;
            }
        }
        sort(next.begin(), next.end()); //排序next
        newRow.move.push_back(next);    //新状态的次态（新一列）加入状态转移表的新一行中
    }

    table.push_back(newRow);        //将新一行（新状态对每个输入的次态）加入状态转移表
    checkNewState(stateNo - 1, nfa);//检查新一行中是否有新状态
}

//为状态转移表排序
void DFAtable::sortTable()
{
    sort(table.begin(), table.end(), DFAcmp);
}

//检查状态转移表中序号为s的行是否存在新状态
void DFAtable::checkNewState(int s, const NFAtable& nfa)
{
    for (int i = 0; i < inputNo; i++)
    {
        //次态非空且未被添加，加入DFA状态中
        if (table[s].move[i].size() && searchState(table[s].move[i]) == -1)
                addState(table[s].move[i], nfa);
    }
}

int main(void)
{
    NFAtable nfa;
    DFAtable dfa;
    string state, input;

    cout << "请输入状态集，请以初态开始" << endl;
    cin >> state;
    cout << "请输入输入集" << endl;
    cin >> input;
    cin.ignore();   //跳过换行符

    nfa = NFAtable(state, input, state.size(), input.size());
    cout << "请输入NFA" << endl;
    nfa.inputMove();
    cout << "输出NFA" << endl;
    nfa.outputTable();

    cout << endl << endl << "进行转换" << endl;
    dfa.addFromNFA(nfa);
    cout << "输出DFA" << endl;
    dfa.outputTable();

    system("pause");
    return 0;
}

//用于NFA状态转移表排序的比较函数
bool NFAcmp(NFAmove a, NFAmove b)
{
    return a.state < b.state;
}

//用于DFA状态转移表排序的比较函数
bool DFAcmp(DFAmove a, DFAmove b)
{
    int lenFlag = a.state.length() < b.state.length();  
    int lenEq = a.state.length() == b.state.length();

    if (lenFlag)     //a的状态长度比b的短
        return true;
    else if (lenEq) //长度一样
    {
        int flag = a.state.compare(b.state);
    
        if (flag > 0)
            return false;
        else if (flag <= 0)
            return true;
    }
    
    return false;   //a的状态长度比b的长
}
