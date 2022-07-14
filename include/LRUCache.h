#ifndef LRUCACHE_H
#define LRUCACHE_H

#include<iostream>
#include<string>
#include<unordered_map>
#include<list>
#include<string>
#include"skiplist.h"
#include"logger.h"

using namespace Thread::Logger;

template<typename K,typename V>
class SkipList;

template<typename KeyT,typename ValueT>
class LRUCache{
    typedef std::pair<KeyT,ValueT> Pair;
    typedef std::list<Pair> List;
    typedef std::unordered_map<KeyT,typename List::iterator> Map;
public:
    LRUCache(int cap,SkipList <KeyT,ValueT>* );
    ~LRUCache();

   bool Put(KeyT key,ValueT value);

   bool Get(KeyT key,ValueT* pValue);

   std::size_t size() const;

    LRUCache(const LRUCache&)=delete;
    LRUCache& operator=(const LRUCache&)=delete;
private:
    int m_capacity;
    List m_list;
    Map m_map;
    SkipList <KeyT,ValueT>* skipList;
};

template<typename KeyT,typename ValueT>
LRUCache<KeyT,ValueT>::LRUCache(int cap,SkipList <KeyT,ValueT>* skipList):
   m_capacity(cap),skipList(skipList)
{
}

template<typename KeyT,typename ValueT>
LRUCache<KeyT,ValueT>::~LRUCache(){
    m_list.clear();
    m_map.clear();
}

template<typename KeyT,typename ValueT>
std::size_t LRUCache<KeyT,ValueT>::size() const{
    return m_list.size();
}

template <class KeyT,class ValueT>
bool LRUCache<KeyT,ValueT>::Put(KeyT key,ValueT value){
    typename Map::iterator iter=m_map.find(key);
    if(iter!=m_map.end()){
        m_list.erase(iter->second);
        m_map.erase(iter);
    }

    m_list.push_front(std::make_pair(key,value));
    m_map[key]=m_list.begin();

    if(m_list.size()>m_capacity){
        KeyT endKey=m_list.back().first;
        skipList->delete_element(endKey);
        m_list.pop_back();
        m_map.erase(endKey);
    }
    return true;
}

template<typename KeyT,typename ValueT>
bool LRUCache<KeyT,ValueT>::Get(KeyT key,ValueT* pValue){
    auto iter=m_map.find(key);
    if(iter==m_map.end()){
        return false;
    }

    typename List::iterator listIter=iter->second;

    std::pair<KeyT,ValueT> listPair=std::make_pair(listIter->first,listIter->second);

    m_list.erase(listIter);

    m_list.push_front(listPair);

    m_map[key]=m_list.begin();
    *pValue=listPair.second;
    
    return true;

}

#endif