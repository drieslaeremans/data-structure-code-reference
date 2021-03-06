/**
CLASS implemented: bag_linked_list (see bag_linked_list.h for documentation)
INVARIANT for the bag ADT:
1. The items in the bag are stored on a linked list;
2. The head pointer of the list is stored in the member variable head_ptr;
3. The total number of items in the list is stored in the member variable
many_nodes.
*/

#include <cassert>  // Provides assert
#include <cstdlib>  // Provides NULL, rand, size_t
#include "node.h"  // Provides node and the linked list functions
#include "bag_linked_list.h"

using namespace std;

bag::bag() {
    head_ptr = nullptr;
    many_nodes = 0;
}

bag::bag(const bag &source) {
    node *tail_ptr;  // Needed for argument of list_copy

    list_copy(source.head_ptr, head_ptr, tail_ptr);
    many_nodes = source.many_nodes;
}

bag::~bag() {
    list_clear(head_ptr);
    many_nodes = 0;
}

bag::size_type bag::count(const value_type &target) const {
    size_type answer = 0;
    // Use const node* since we don't change the nodes.
    const node *cursor = list_search(head_ptr, target);
    while (cursor != nullptr) {
        // Each time that cursor is not nullptr, we have another occurrence of
        // target, so we add one to answer, and move cursor to the next
        // occurrence of the target.
        ++answer;
        cursor = cursor->link();
        cursor = list_search(cursor, target);
    }
    return answer;
}

bag::size_type bag::erase(const value_type &target) {
    size_type answer = 0;
    node *target_ptr = list_search(head_ptr, target);
    while (target_ptr != nullptr) {
        // Each time that target_ptr is not nullptr, we have another occurrence
        // of target. We remove this target using the same technique that
        // was used in erase_one.
        target_ptr->set_data(head_ptr->data());
        target_ptr = target_ptr->link();
        target_ptr = list_search(target_ptr, target);
        list_head_remove(head_ptr);
        --many_nodes;
        ++answer;
    }
    return answer;
}

bool bag::erase_one(const value_type &target) {
    node *target_ptr;

    target_ptr = list_search(head_ptr, target);
    if (target_ptr == nullptr)
        return false; // target isn't in the bag, so no work to do
    target_ptr->set_data(head_ptr->data());
    list_head_remove(head_ptr);
    --many_nodes;
    return true;
}

bag::value_type bag::grab() const {
    size_type i;
    const node *cursor; // Use const node* since we don't change the nodes.

    assert(size() > 0);
    i = (rand() % size()) + 1;
    cursor = list_locate(head_ptr, i);
    return cursor->data();
}

void bag::insert(const value_type &entry) {
    list_head_insert(head_ptr, entry);
    ++many_nodes;
}

void bag::operator+=(const bag &addend) {
    node *copy_head_ptr;
    node *copy_tail_ptr;

    if (addend.many_nodes > 0) {
        list_copy(addend.head_ptr, copy_head_ptr, copy_tail_ptr);
        copy_tail_ptr->set_link(head_ptr);
        head_ptr = copy_head_ptr;
        many_nodes += addend.many_nodes;
    }
}

void bag::operator=(const bag &source) {
    node *tail_ptr; // Needed for argument to list_copy

    if (this == &source)
        return;

    list_clear(head_ptr);
    many_nodes = 0;
    list_copy(source.head_ptr, head_ptr, tail_ptr);
    many_nodes = source.many_nodes;
}

bag operator+(const bag &b1, const bag &b2) {
    bag answer;

    answer += b1;
    answer += b2;
    return answer;
}
