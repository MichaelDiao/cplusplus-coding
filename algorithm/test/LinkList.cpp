/******************************************************************************
* File:             LinkList.cpp
* Author:           链表相关的算法  
* Created:          03/25/20 
* Description:      
*****************************************************************************/

#include<iostream>
#include <ctime>
#include <map>
#include "list.h"

using namespace std;

/*****************************************************************
* @brief: 从单链表中获取倒数第N个节点
* @param: 
* head: 链表的头结点
* N 倒数第N个节点
* @retrun:  retrun
* @comment:  遍历法
*****************************************************************/
Node* getNodeFromBottom(Node* head, int N){
    if(N <= 0 || !head->_next){
        return nullptr;
    }

    int cnt = 0;
    Node* cur_node = head;

    while(cur_node->_next){
        cur_node = cur_node->_next;
        ++cnt;
    }

    int i = 0;
    cur_node = head;
    while(cur_node->_next && i <= cnt-N){
        cur_node = cur_node->_next;
        ++i;
    }

    return cur_node;
}

/*****************************************************************
* @brief: 从单链表中获取倒数第N个节点
* @param: 
* head: 链表的头结点
* N 倒数第N个节点
* @retrun:  retrun
* @comment:  递归法
*****************************************************************/
Node* getNodeFromBottom2(Node* node, int N){
    static int cnt = N;
    if(!node){
        return nullptr;
    }

    Node* cur = getNodeFromBottom2(node->_next, N);
    if(cur){
        return cur;
    }else{
        if(--cnt == 0){
            return node;
        }
    }

    return nullptr;
}

/*****************************************************************
* @brief: 从单链表中获取倒数第N个节点
* @param: 
* head: 链表的头结点
* N 倒数第N个节点
* @retrun:  retrun
* @comment:  双指针法，两个指针相距N-1，快指针到达链表尾部时，慢指针
* 就是倒数第N各节点
*****************************************************************/
Node* getNodeFromBottom3(Node* node, int N){
    if(N <= 0 || !node ){
        return nullptr;
    }
    Node* fast = node;
    Node* slow = node;
    int i=0;
    while(i++ < N-1 && fast->_next){
        fast = fast->_next;
    }
    if(i<N){
        return nullptr;
    }

    while(fast->_next){
        fast = fast->_next;
        slow = slow->_next;
    }

    return slow;
}

//链表获取倒数第N个节点的实现方法
//遍历法，递归法，双指针法，其中双指针最快，其次遍历，最慢递归
void getNodeFromBottom_test(){
    clock_t star, end;
    SingleLinkList sl(1200);
    sl.show();
    Node* head = nullptr;
    sl.get_head(head);

    //递归法最慢，链表越长，耗时越多
    cout << "recursion: " << endl;
    star = clock();
    Node* node = getNodeFromBottom2(head, 459);
    end = clock();
    cout << "node: " << node->_next << ", data: " << node->_data << endl;
    cout << "cost time: " << (double)(end-star)/CLOCKS_PER_SEC * 1000000 << "us." << endl;
    
    //遍历法当链表长度较小时，基本与双指针法速度相当
    cout << "-------------------" << endl;
    cout << "through linklist: " << endl;
    star = clock();
    Node* node2 = getNodeFromBottom(head, 459);
    end = clock();
    cout << "node: " << node2->_next << ", data: " << node2->_data << endl;
    cout << "cost time: " << (double)(end-star)/CLOCKS_PER_SEC * 1000000 << "us." << endl;

    //双指针最快
    cout << "-------------------" << endl;
    cout << "double pointers: " << endl;
    star = clock();
    Node* node3 = getNodeFromBottom3(head, 459);
    end = clock();
    cout << "node: " << node3->_next << ", data: " << node3->_data << endl;
    cout << "cost time: " << (double)(end-star)/CLOCKS_PER_SEC * 1000000 << "us." << endl;

}


/*****************************************************************
* @brief: 快慢指针法
* @param: 
* head: 链表头结点
* @retrun:  retrun
* @comment:  -
*****************************************************************/
bool isExsistLoop_fastslow(Node* head){
    Node* fast = head->_next;
    Node* slow = head;
    
    while(fast != slow){
        if(fast){
            fast = fast->_next;
        }
        if(fast){
            fast = fast->_next;
        }else{
            break;
        }
        slow = slow->_next;
    }

    return fast ? true : false;
}
/*****************************************************************
* @brief: 哈希缓存法
* @param: 
* head: 链表头结点
* @retrun:  retrun
* @comment:  -
*****************************************************************/
bool isExsistLoop_hash(Node* head){
    Node* node = head;
     
    map<Node*, int> addr;
    
    while(node->_next){
        node = node->_next;
        if(addr.find(node) == addr.end()){
            addr[node] = 1;
        }else{
            return true;
        }
    }

    return false;
}

//判断链表中是否有环的问题
//两种方案：快慢指针和哈希缓存
void isExsistLoop_test(){
    SingleLinkList sl(10);
    sl.show();
    Node* head = nullptr;
    sl.get_head(head);

    //构造一个有环链表
    Node* crossNode = getNodeFromBottom3(head, 4);
    Node* tailNode = getNodeFromBottom3(head, 1);
    tailNode->_next = crossNode;

    if(isExsistLoop_fastslow(head)){
    /* if(isExsistLoop_hash(head)){ */
        cout << "loop exsist!" << endl;
    }else{
        cout << "loop not exsist!" << endl;
    }

    //测试完成后，需要手动把这个环打开，否则析构会出问题
    tailNode->_next = nullptr;
}

/*****************************************************************
* @brief: 获取链表的环入口
* @param: 
* head: 链表头
* @retrun:  如果存在环，则返回环入口节点，否则返回nullptr
* @comment:  快慢指针法
*****************************************************************/
Node* getEntryNodeOfLoop_fastslow(Node* head){
    Node* fast = head;
    Node* slow = head;

    Node* mettingNode = nullptr;
    while(slow && fast->_next){
        fast = fast->_next->_next;
        slow = slow->_next;
        //loop exsist
        if(fast == slow){
            mettingNode = fast;
            break;
        }
    }
    //no loop
    if(!fast->_next){
        return nullptr;
    }

    Node* p1 = mettingNode;
    Node* p2 = head;
    //二者相遇时就是环的入口节点
    while(p1 != p2){
        p1 = p1->_next;
        p2 = p2->_next;
    }

    return p1;
}

/*****************************************************************
* @brief: 获取链表的环入口
* @param: 
* head: 链表头
* @retrun:  如果存在环，则返回环入口节点，否则返回nullptr
* @comment:  哈希缓存法
*****************************************************************/
Node* getEntryNodeOfLoop_hash(Node* head){
    map<Node*, int> addr;
    Node* cur = head;
    while(cur->_next){
        cur = cur->_next;
        if(addr.find(cur) == addr.end()){
            addr[cur] = 1;
        }else{
            return cur;
        }
    }
    
    return nullptr;
}

//判断链表是否有环，若有，则返回环的入口
//两种方案：快慢指针和哈希缓存，哈希缓存容易理解，思路简单
//哈希缓存法速度快,但是占用内存大,因为需要一个与链表同等长度的map
//快慢指针 速度慢,因为需要遍历多次链表,但占用内存小,不需要额外的内存空间
void getEntryNodeOfLoop_test(){
    SingleLinkList sl(10);
    sl.show();
    Node* head = nullptr;
    sl.get_head(head);

    //构造一个有环链表
    Node* crossNode = getNodeFromBottom3(head, 4);
    Node* tailNode = getNodeFromBottom3(head, 1);
    tailNode->_next = crossNode;
    cout << "tail next data: " << tailNode->_next->_data << endl;

    /* Node* entry = getEntryNodeOfLoop_fastslow(head); */
    Node* entry = getEntryNodeOfLoop_hash(head);
    if(entry){
        cout << "loop exsist: data=" << entry->_data << endl;
    }else{
        cout << "loop do not exsist." << endl;
    }


    //测试完成后，需要手动把这个环打开，否则析构会出问题
    tailNode->_next = nullptr;

}

int main(int argc, char *argv[])
{
    //链表获取倒数第N个节点
    /* getNodeFromBottom_test(); */

    //链表是否存在环
    /* isExsistLoop_test(); */

    //链表是否存在环，若存在找环的入口
    getEntryNodeOfLoop_test();


    return 0;
}