export module big_int;

import std;
import doubly_linked_list;

export class BigInt final
{
private:
    using Digits = DoublyLinkedList<std::uint8_t>;

    int sign_ = 0;
    Digits body_;

public:
};