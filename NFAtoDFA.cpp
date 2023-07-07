#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
using namespace std;

//�洢NFA��һ��״̬�ڽ��������Ĵ�̬
typedef struct
{
    char state;
    vector<string> move;
}NFAmove;

//�洢DFA��һ��״̬�ڽ��������Ĵ�̬
typedef struct
{
    string state;
    vector<string> move;
}DFAmove;

//�ֱ�����NFA��DFA״̬ת�Ʊ�����ıȽϺ���
bool NFAcmp(NFAmove a, NFAmove b);
bool DFAcmp(DFAmove a, DFAmove b);

//NFA״̬ת�Ʊ���
class NFAtable
{
    public:
        NFAtable();
        //���캯����sΪ״̬����Ĭ��һ���ǿ��ַ�Ϊһ��״̬��in�����뼯��Ĭ��һ���ǿ��ַ�Ϊһ�����롣
        //sNo��״̬����С��iNo�����뼯��С
        NFAtable(const string& s, const string& in, int sNo, int iNo);  
        ~NFAtable();

        int searchState(char s) const;  //����״̬s��״̬ת�Ʊ������ţ�û���򷵻�-1
        void sortTable();               //Ϊ״̬ת�Ʊ����������
        void inputMove();               //�����û�����״̬��ÿ������Ĵ�̬
        void outputTable() const;       //��ʽ�����״̬ת�Ʊ�

    public:
        vector<char> input;     //���뼯��Ĭ��һ���ǿ��ַ�Ϊһ������
        vector<NFAmove> table;  //״̬ת�Ʊ�Ĭ��һ���ǿ��ַ�Ϊһ��״̬����̬������
        int stateNo;            //״̬����
        int inputNo;            //���뼯��С
};

NFAtable::NFAtable()
    : stateNo(0)
    , inputNo(0)
{
    //Ĭ�Ϲ��캯����NFA״̬ת�Ʊ���Ϊ��
    input.clear();
    table.clear();
};

//���캯����sΪ״̬����Ĭ��һ���ǿ��ַ�Ϊһ��״̬��in�����뼯��Ĭ��һ���ǿ��ַ�Ϊһ������״̬��
//sNo��״̬����С��iNo�����뼯��С
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
        input.push_back(in[i]); //�������뼯
    }

    for (int i = 0; i < stateNo; i++)
    {
        //����״̬������״̬ת�Ʊ�
        m.state = s[i];         
        table.push_back(m);
    }
}

NFAtable::~NFAtable()
{};

//����״̬s��״̬ת�Ʊ������ţ�û���򷵻�
int NFAtable::searchState(char s) const
{
    int found = -1;

    //�ҵ�״̬�����������к�
    for (int i = 0; i < stateNo && found == -1; i++)
    {
        if (s == table[i].state)
            found = i;
    } 

    return found;
}

//Ϊ״̬ת�Ʊ����������
void NFAtable::sortTable()
{
    sort(table.begin(), table.end(), NFAcmp);
}

//�����û�����״̬��ÿ�������ת��
void NFAtable::inputMove()
{
    string m;
    for (int i = 0; i < stateNo; i++)
    {
        for(int j = 0; j < inputNo; j++)
        {
            //������ʾ
            cout << "״̬" << table[i].state << "����" << input[j] << ':' << endl;
            getline(cin, m);            //�������룬�Ի��н��������û��ת����ֱ�ӻ��С�
            sort (m.begin(), m.end());  //����̬����
            table[i].move.push_back(m); //��ӵ�ת�Ʊ�
        }
    }
    sortTable();    //��ת�Ʊ�����
}

//��ʽ�����״̬ת�Ʊ�
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
            cout << right << setw(10) << ((table[i].move[j].empty()) ? "NULL" : table[i].move[j]); //ת�Ƶ���һ̬����Ϊ������NULL��ʾ
        }
        cout << endl;
    }
}

//DFA״̬ת�Ʊ���
class DFAtable
{
    public:
        DFAtable();
        ~DFAtable();

        void addFromNFA(const NFAtable& nfa);       //��NFA nfa�����״̬
        int searchState(const string& s) const;     //����״̬s��ת�Ʊ���кţ�û���򷵻�-1
        void outputTable() const;                   //��ʽ�����DFA״̬ת�Ʊ�   

    private:
        void addState(const string& s, const NFAtable& nfa);    //����״̬s��ӽ�DFA��״̬ת�Ʊ��Ӳ���nfa��ȡ��̬
        void sortTable();                                       //Ϊ״̬ת�Ʊ�����
        void checkNewState(int s, const NFAtable& nfa);         //���״̬ת�Ʊ������Ϊs�����Ƿ������״̬

    public:
        vector<char> input;     //���뼯��Ĭ��һ���ǿ��ַ�Ϊһ������
        vector<DFAmove> table;  //״̬ת�Ʊ�һ���ǿ�stringΪһ��״̬����̬������
        int stateNo;            //״̬���� 
        int inputNo;            //���뼯��С
};

DFAtable::DFAtable()
    : stateNo(0)
    , inputNo(0)
{
    //Ĭ�Ϲ��캯����DFA״̬ת�Ʊ���Ϊ��
    input.clear();
    table.clear();
};

DFAtable::~DFAtable()
{};

//��NFA nfa�����״̬
void DFAtable::addFromNFA(const NFAtable& nfa)
{
    //���뼯��nfa��һ��
    input = nfa.input;
    inputNo = nfa.inputNo;

    //�ӳ�̬��ʼ���
    //ֻ��Ӵӳ�̬�ɴ��״̬
    stateNo = 1;    //״̬��Ϊ1
    DFAmove m;
    string s(1, nfa.table[0].state);    //��ȡ��̬
    vector<string> n(nfa.table[0].move);//��ȡ��̬��ÿ������Ĵ�̬
    m.move = n;
    m.state = s;
    table.push_back(m);                 //����ת�Ʊ������

    for (int i = 0; i < inputNo; i++)
    {
        //ֻ�е���̬�Ĵ�̬�ǿ���δ�����ʱ���ż���
        if (table[0].move[i].size() && searchState(table[0].move[i]) == -1)
                addState(table[0].move[i], nfa);
    }

    //����״̬ת�Ʊ�
    sortTable();
}

//����״̬s��ת�Ʊ���кţ�û���򷵻�-1
int DFAtable::searchState(const string& s) const
{
    int found = -1;

    //�ҵ�״̬�����������к�
    for (int i = 0; i != table.size() && found == -1; i++)
    {
        if (!s.compare(table[i].state))
            found = i;
    }

    return found;
}

//��ʽ�����DFA״̬ת�Ʊ�
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
            cout << right << setw(10) << ((table[i].move[j].empty()) ? "NULL" : table[i].move[j]);  //ת�Ƶ���һ̬����Ϊ������NULL��ʾ
        }
        cout << endl;
    }
}

//����״̬s��ӽ�DFA��״̬ת�Ʊ��Ӳ���nfa��ȡ��һ̬
void DFAtable::addState(const string& s, const NFAtable& nfa)
{
    int len = s.size();   //��״̬�ĳ���
    DFAmove newRow;

    newRow.state = s;
    stateNo++;              //״̬����һ

    for (int i = 0; i < inputNo; i++)
    {
        string next;    //��״̬�Ե�ǰ���Ϊi������Ĵ�̬
        for (int j = 0; j < len; j++)
        {
            int row = nfa.searchState(s[j]);                //��ȡ��״̬�У�����״̬���ַ�����NFA�������
            string singleStateNext(nfa.table[row].move[i]); //�õ���״̬��ÿ������Ĵ�̬
            for (int k = 0; k < singleStateNext.length(); k++)
            {
                //�����̬δ����ӣ������next
                if (next.find(singleStateNext[k]) == -1)
                    next.push_back(singleStateNext[k]);
                else if (next.find(singleStateNext) != -1)
                    break;
            }
        }
        sort(next.begin(), next.end()); //����next
        newRow.move.push_back(next);    //��״̬�Ĵ�̬����һ�У�����״̬ת�Ʊ����һ����
    }

    table.push_back(newRow);        //����һ�У���״̬��ÿ������Ĵ�̬������״̬ת�Ʊ�
    checkNewState(stateNo - 1, nfa);//�����һ�����Ƿ�����״̬
}

//Ϊ״̬ת�Ʊ�����
void DFAtable::sortTable()
{
    sort(table.begin(), table.end(), DFAcmp);
}

//���״̬ת�Ʊ������Ϊs�����Ƿ������״̬
void DFAtable::checkNewState(int s, const NFAtable& nfa)
{
    for (int i = 0; i < inputNo; i++)
    {
        //��̬�ǿ���δ����ӣ�����DFA״̬��
        if (table[s].move[i].size() && searchState(table[s].move[i]) == -1)
                addState(table[s].move[i], nfa);
    }
}

int main(void)
{
    NFAtable nfa;
    DFAtable dfa;
    string state, input;

    cout << "������״̬�������Գ�̬��ʼ" << endl;
    cin >> state;
    cout << "���������뼯" << endl;
    cin >> input;
    cin.ignore();   //�������з�

    nfa = NFAtable(state, input, state.size(), input.size());
    cout << "������NFA" << endl;
    nfa.inputMove();
    cout << "���NFA" << endl;
    nfa.outputTable();

    cout << endl << endl << "����ת��" << endl;
    dfa.addFromNFA(nfa);
    cout << "���DFA" << endl;
    dfa.outputTable();

    system("pause");
    return 0;
}

//����NFA״̬ת�Ʊ�����ıȽϺ���
bool NFAcmp(NFAmove a, NFAmove b)
{
    return a.state < b.state;
}

//����DFA״̬ת�Ʊ�����ıȽϺ���
bool DFAcmp(DFAmove a, DFAmove b)
{
    int lenFlag = a.state.length() < b.state.length();  
    int lenEq = a.state.length() == b.state.length();

    if (lenFlag)     //a��״̬���ȱ�b�Ķ�
        return true;
    else if (lenEq) //����һ��
    {
        int flag = a.state.compare(b.state);
    
        if (flag > 0)
            return false;
        else if (flag <= 0)
            return true;
    }
    
    return false;   //a��״̬���ȱ�b�ĳ�
}
