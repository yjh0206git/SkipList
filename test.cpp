#include "include/logger.h"
#include "include/thread_pool.h"
#include "include/skiplist.h"
#include<iostream>
#include<functional>
#include<cmath>

using namespace std;
using namespace Thread::Logger;

template<typename Derived>
void fun(BaseLogger<Derived>& b){
    b.end_line(Level::Info,"successful");
}

void test(){
    auto& f=ConsoleLogger::get_instance();
    fun(f);
    auto& f2=FileLogger::get_instance();
    fun(f2);
}

void test2(){
    auto&& debug=ConsoleLogger::get_instance();

    auto is_prime=[](int n)->bool{
        if(n<=1) return false;
        int sqrt_n=sqrt(n);
        for(int i=2;i<=sqrt_n;++i){
            if(n%i==0) return false;
        }
        return true;
    };

    auto prime_count=[&is_prime,&debug](int& ans,int l,int r){
        int cnt=0;
        for(int i=l;i<=r;++i){
            cnt+=is_prime(i);
        }

        __sync_fetch_and_add(&ans,cnt);
        debug(Level::Info)<<"This Thread "<<"id: "<<std::this_thread::get_id()
                    <<" count is "<<cnt;
        
    };

    Thread::thread_pool tp((size_t)5);
    tp.start();

    int ans=0;
    const int N=10000;
    for(int i=0;i<10;i++){
        tp.add_one_task(prime_count,std::ref(ans),i*N+1,(i+1)*N);
    }

    tp.stop_until_empty();
    cout<<ans<<endl;
}

void testSkipList(){
    
    SkipList<int, std::string> skipList(6);
	skipList.insert_element(1, "学"); 
	skipList.insert_element(3, "算法"); 
	skipList.insert_element(7, "认准"); 
	skipList.insert_element(8, "微信公众号：代码随想录"); 
	skipList.insert_element(9, "学习"); 
	skipList.insert_element(19, "算法不迷路"); 
	skipList.insert_element(19, "赶快关注吧你会发现详见很晚！"); 

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.dump_file();

    // skipList.load_file();

    skipList.search_element(9);
    skipList.search_element(18);


    skipList.display_list();

    skipList.delete_element(3);
    skipList.delete_element(7);

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.display_list();

}

int main(){
    //test();
    testSkipList();
    return 0;
}