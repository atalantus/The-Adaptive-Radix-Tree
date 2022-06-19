#include "Benchmark.h"

using namespace std;

int main(int argc, char* argv[])
{
	auto t = new Trie();

    t->insert(0);
    t->insert(1);
    t->insert(128);
    t->insert(240);
    t->insert(4294967295); // 2^32-1

    cout << t->find(0) << endl;
    cout << t->find(1) << endl;
    cout << t->find(128) << endl;
    cout << t->find(240) << endl;
    cout << t->find(4294967295) << endl; // 2^32-1

    cout << t->find(2) << endl;
    cout << t->find(3) << endl;
    cout << t->find(3234234) << endl;
    cout << t->find(323423424) << endl;
    cout << t->find(17) << endl;
}
