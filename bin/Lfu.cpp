//
// Created by mohit on 20/2/17.
//

#include <iostream>
#include "../header/Lfu.h"

Lfu::Lfu() {
    this->head = new FrequencyList(0, NULL, NULL);
    this->CountFrequencyListMap[0] = this->head;
}

Lfu::~Lfu() {
    delete(this->head);
    this->NodeFrequencyListMap.clear();
    this->CountFrequencyListMap.clear();
}



void Lfu::NodeAlreadyPresent(int node){
    unordered_map<int, FrequencyList *>::const_iterator iter1 = this->NodeFrequencyListMap.find(node);
    FrequencyList *frequencyList = iter1->second;
    int freq = frequencyList->GetCount();
    // remove the node from current frequency node.
    frequencyList->DeleteNode(node);

    // Increment the frequency
    freq = freq + 1;
    // check if the node is present.
    if(FrequencyNodePresent(freq)){
        FrequencyList *frequencyList1 = this->CountFrequencyListMap.find(freq)->second;
        frequencyList1->AddNode(node);
        this->NodeFrequencyListMap[node] = frequencyList1;
    }else{
        FrequencyList *newFrequencyNode = new FrequencyList(freq);
        newFrequencyNode->SetPrevious(frequencyList);
        newFrequencyNode->SetNext(frequencyList->GetNext());
        this->CountFrequencyListMap[freq] = newFrequencyNode;
        frequencyList->SetNext(newFrequencyNode);

        // adjust the pointers accordingly.
        frequencyList->GetNext()->SetPrevious(newFrequencyNode);
        frequencyList->SetNext(newFrequencyNode);

        newFrequencyNode->AddNode(node);
        this->NodeFrequencyListMap[node] = newFrequencyNode;
    }

    if(IsNodeEmpty(frequencyList)){
        frequencyList->GetNext()->SetPrevious(frequencyList->GetPrevious());
        frequencyList->GetPrevious()->SetNext(frequencyList->GetNext());
        this->CountFrequencyListMap.erase(frequencyList->GetCount());
        free(frequencyList);
    }
}
void Lfu::NewNodeSet(int node) {
    // else put it the first node.
    int freq = 1;
    if(FrequencyNodePresent(freq)){
        FrequencyList *frequencyList1 = this->CountFrequencyListMap.find(freq)->second;
        frequencyList1->AddNode(node);
        this->NodeFrequencyListMap[node] = frequencyList1;
    }else{
        FrequencyList *newFrequencyNode = new FrequencyList(freq);
        this->CountFrequencyListMap[freq] = newFrequencyNode;
        newFrequencyNode->SetPrevious(this->head);
        newFrequencyNode->SetNext(this->head->GetNext());
        newFrequencyNode->AddNode(node);
        // make next of head as new frequency node
        this->head->SetNext(newFrequencyNode);
        if(newFrequencyNode->GetNext()!=NULL)
            newFrequencyNode->GetNext()->SetPrevious(newFrequencyNode);
        this->NodeFrequencyListMap[node] = newFrequencyNode;
    }
}

void Lfu::Set(int value) {
    // add the item in the cache.
    // Check if the item is already present. If yes increment the count and put it in the next frequency node.
    int node = value;
    if(NodePresent(node)){
        NodeAlreadyPresent(node);
    }else{
        NewNodeSet(node);
    }

}

void Lfu::Retrieve(int value) {
    int node = value;
    if(NodePresent(node)){
        NodeAlreadyPresent(node);
    }else{
        // Dont Know if new node has to be added!!
        NewNodeSet(node);
    }
}


void Lfu::EvictFromCache() {
    FrequencyList *root = this->head;
    if (root->GetNext() == NULL)
    {
        return;
    }
    FrequencyList *nextNode = root->GetNext();
    unordered_map<int,int>::iterator it = nextNode->GetNodeList().begin();
    nextNode->DeleteNode(it->first);
    this->NodeFrequencyListMap.erase(it->first);
    if (nextNode->IsEmpty())
    {
        this->CountFrequencyListMap.erase(nextNode->GetCount());
        if (nextNode->GetPrevious() != NULL)
            nextNode->GetPrevious()->SetNext(nextNode->GetNext());
        if (nextNode->GetNext() != NULL)
            nextNode->GetNext()->SetPrevious(nextNode->GetPrevious());
        free(nextNode);
    }
}

void Lfu::Evict(int value) {
    int node = value;
    if(NodePresent(node)){
        FrequencyList *presentFrequencyNode = this->NodeFrequencyListMap[node];
        this->NodeFrequencyListMap.erase(node);
        presentFrequencyNode->DeleteNode(node);

        if(IsNodeEmpty(presentFrequencyNode)){
            if(presentFrequencyNode->GetNext()!=NULL)
                presentFrequencyNode->GetNext()->SetPrevious(presentFrequencyNode->GetPrevious());
            if(presentFrequencyNode->GetPrevious()!=NULL)
                presentFrequencyNode->GetPrevious()->SetNext(presentFrequencyNode->GetNext());
            this->CountFrequencyListMap.erase(presentFrequencyNode->GetCount());
            free(presentFrequencyNode);
        }

    }else{
        /**
         * Return Element is not present.
         * */
    }
}

bool Lfu::NodePresent(int node) {
    unordered_map<int, FrequencyList *>::const_iterator iter1 = this->NodeFrequencyListMap.find(node);
    return !(iter1 == this->NodeFrequencyListMap.end());
}

bool Lfu::FrequencyNodePresent(int count) {
    unordered_map<int, FrequencyList *>::const_iterator iter1 = this->CountFrequencyListMap.find(count);
    return !(iter1 == this->CountFrequencyListMap.end());
}

bool Lfu::IsNodeEmpty(FrequencyList *frequencyList) {
    return frequencyList->IsEmpty();
}

void Lfu::PrintLfu() {
    FrequencyList *Node = this->head;
    while(Node!=NULL){
        cout<<Node->GetCount()<<" :: Count "<<endl;
        Node->PrintNodeList();
        Node = Node->GetNext();
        cout<<"--------------\n";
    }
}

/*int main(){
    Lfu *lfu = new Lfu();
    lfu->Set(1);
    lfu->Set(1);
    lfu->Set(2);
    lfu->Retrieve(2);
    lfu->Set(3);
    lfu->Evict(3);
    lfu->Set(1);
    lfu->Set(4);
    lfu->PrintLfu();
    return 0;
}
*/