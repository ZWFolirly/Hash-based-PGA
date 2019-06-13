#include <iostream>
#include "stdio.h"
#include "Graph.h"
#include "config.h"
#include "thread"
#include "mutex"
#include "PGenetic.h"
#include "PHashGenetic.h"

using namespace std;
timed_mutex ask_hash_mutex;
string filename = "lscp400.txt";
Graph g = Graph("..\\" + filename);
timed_mutex best_mutex;
timed_mutex hash_best_mutex;

void exec_hash() {
    int *hash = new int[HASH_SIZE];
    int *timestamp = new int[HASH_SIZE];
//等待链接
//若是询问
//    等待其他线程询问
//    查表，返回适应度，或者null
//若是告知插入操作
//    将值插入表中
//继续
}

void exec_para_hash_gene(int i, bool **best, int *hash, int *timestamp) {
//初始化
//结束条件：MAX_GEN
    //获得种群
//    询问hash，顺便获得较优的个体
    cout << "Now play on thread " << i << endl;
    string hpga_filename = "hpga[" + to_string(SEED) + "][" + to_string(POP_NUM) + "]" + filename;
    PHashGenetic hpga = PHashGenetic(g, "..\\" + to_string(i) + hpga_filename);
    hpga.e.seed(i);


    best[i] = new bool[hpga.row];
    bool **get = new bool *[THREAD_NUM];
    for (int k = 0; k < THREAD_NUM; ++k) {
        get[k] = new bool[hpga.row];
    }
    bool *is_changed = new bool[THREAD_NUM];
    time_t now;
    for (hpga.generation = 0; hpga.generation < MAX_GEN; ++hpga.generation) {
        for (int j = 0; j < POP_NUM; ++j) {
            if (hpga.u_1(hpga.e) < hpga.cross_rate)
                hpga.geneCrossover(j);
            if (hpga.u_1(hpga.e) < hpga.mutation_rate)
                hpga.geneMutation(j);
        }

        time(&now);
        //cout << best_fit << endl;
        if (hpga.generation % 100 == 0) {
            cout << "[" << i << "] generation:" << hpga.generation << "\ttime costs:" << (now - hpga.st)
                 << "s\tbest fit:" << hpga.best_fit << "\taccuracy num:" << hpga.accuracy_rate << "\thash size:"
                 << hpga.hash_size << "\n";


            while (!hash_best_mutex.try_lock_for(chrono::milliseconds(200))) {
                cout << "Thread " << i << " try to share\n";
            }

            //同步hash表
            for (int n = 0; n < HASH_SIZE; ++n) {
                if (timestamp[n] != -1 && hpga.timestamp[n] != -1) {
                    if (hash[n] < hpga.hash[n]) {
                        hpga.hash[n] = hash[n];
                        hpga.timestamp[n] = timestamp[n];
                    } else if (hash[n] > hpga.hash[n]) {
                        hash[n] = hpga.hash[n];
                        timestamp[n] = hpga.timestamp[n];
                    }
                } else if (timestamp[n] != -1 && hpga.timestamp[n] == -1) {
                    ++hpga.hash_size;
                    hpga.hash[n] = hash[n];
                    hpga.timestamp[n] = timestamp[n];
                } else if (timestamp[n] == -1 && hpga.timestamp[n] != -1) {
                    hash[n] = hpga.hash[n];
                    timestamp[n] = hpga.timestamp[n];
                }
            }

            for (int l = 0; l < hpga.row; ++l) {
                best[i][l] = hpga.best_gen[l];
            }

            for (int j = 0; j < THREAD_NUM; ++j) {
//                //is_changed[j] = false;
//                //if (j == i) continue;
//                for (int k = 0; k < hpga.row; ++k) {
//                    //if (get[j][k] != best[j][k]) {
//                        is_changed[j] = true;
//                        break;
//                    }
//                }
//                if (is_changed[j]) {
                    for (int k = 0; k < hpga.row; ++k)
                        get[j][k] = best[j][k];
                //}
            }
            //cout << "Thread " << i << " shared\n";
            hash_best_mutex.unlock();
            for (int m = 0; m < THREAD_NUM; ++m) {
                //if(is_changed[m]){
                    int pos = hpga.u_pop(hpga.e);
                    for (int j = 0; j < hpga.row; ++j) {
                        hpga.population[pos][j] = get[m][j];
                    }
                    hpga.fitness_degree[pos] = hpga.fitnessFun(hpga.population[pos]);
                //}
            }
        }

        //cout << "generation:" << i << "\ttime costs:" << (now - st)  << "s\n";
        hpga.out << "thread:" << i << "\tgeneration:" << hpga.generation << "\ttime costs:" << (now - hpga.st)
                 << "s\tbest fit:" << hpga.best_fit << "\taccuracy_num:"<<hpga.accuracy_rate << "\thash_size:"<<hpga.hash_size<<"\n";
        //showBest(nullptr, 0);
    }
    time(&now);
    cout << hpga.best_fit << endl << "\ttime costs:" << (now - hpga.st) << "s\n";

//    计算适应度
//    告诉hash
//    继续获得种群

}

void exec_para_gene(int i, bool **best) {
    cout << "Now play on thread " << i << endl;
    string pga_filename = "pga[" + to_string(SEED) + "][" + to_string(POP_NUM) + "]" + filename;
    PGenetic pga = PGenetic(g, "..\\" + to_string(i) + pga_filename);
    pga.e.seed(i);
    bool *is_changed = new bool[THREAD_NUM];
    best[i] = new bool[pga.row];
    bool **get = new bool *[THREAD_NUM];
    for (int k = 0; k < THREAD_NUM; ++k) {
        get[k] = new bool[pga.row];
    }


    time_t now;
    for (pga.generation = 0; pga.generation < MAX_GEN; ++pga.generation) {
        for (int j = 0; j < POP_NUM; ++j) {
            if (pga.u_1(pga.e) < pga.cross_rate)
                pga.geneCrossover(j);
            if (pga.u_1(pga.e) < pga.mutation_rate)
                pga.geneMutation(j);
        }

        time(&now);
        //cout << best_fit << endl;
        if (pga.generation % 100 == 0) {
            cout << "[" << i << "] generation:" << pga.generation << "\ttime costs:" << (now - pga.st)
                 << "s\tbest fit:" << pga.best_fit << "\n";


            while (!best_mutex.try_lock_for(chrono::milliseconds(200))) {
                cout << "Thread " << i << " try to share\n";
            }

            for (int l = 0; l < pga.row; ++l) {
                best[i][l] = pga.best_gen[l];
            }
            for (int j = 0; j < THREAD_NUM; ++j) {
//                is_changed[j] = false;
//                if (j == i) continue;
//                for (int k = 0; k < pga.row; ++k) {
//                    if (get[j][k] != best[j][k]) {
//                        is_changed[j] = true;
//                        break;
//                    }
//                }
//                if (is_changed[j]) {
                    for (int k = 0; k < pga.row; ++k)
                        get[j][k] = best[j][k];
                //}
            }
            //cout << "Thread " << i << " shared\n";
            best_mutex.unlock();
            for (int m = 0; m < THREAD_NUM; ++m) {
                //if(is_changed[m]){
                    int pos = pga.u_pop(pga.e);
                    for (int j = 0; j < pga.row; ++j) {
                        pga.population[pos][j] = get[m][j];
                    }
                    pga.fitness_degree[pos] = pga.fitnessFun(pga.population[pos]);
                //}
            }
        }

        //cout << "generation:" << i << "\ttime costs:" << (now - st)  << "s\n";
        pga.out << "thread:" << i << "\tgeneration:" << pga.generation << "\ttime costs:" << (now - pga.st)
                << "s\tbest fit:" << pga.best_fit << "\n";
        //showBest(nullptr, 0);
    }
    time(&now);
    cout << pga.best_fit << endl << "\ttime costs:" << (now - pga.st) << "s\n";

}

int main() {

    thread hash_gene_t[THREAD_NUM];
    bool **hash_best = new bool *[THREAD_NUM];
    int *hash = new int[HASH_SIZE];
    int *timestamp = new int[HASH_SIZE];
    for (int j = 0; j < HASH_SIZE; ++j) {
        timestamp[j] = -1;
    }
    for (int k = 0; k < THREAD_NUM; ++k) {
        hash_gene_t[k] = thread(exec_para_hash_gene, k, hash_best, hash, timestamp);
    }
    for (auto &it:hash_gene_t)
        it.join();

//    thread gene_t[THREAD_NUM];
//    bool **best = new bool *[THREAD_NUM];
//    for (int i = 0; i < THREAD_NUM; ++i) {
//        gene_t[i] = thread(exec_para_gene, i, best);
//    }
//    for (auto &it:gene_t)
//        it.join();


    return 0;
}