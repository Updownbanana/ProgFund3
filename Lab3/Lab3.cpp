#include <iostream>
#include <cassert>
#include <string>
#include <stdexcept>

using namespace std;

template<typename T>
class ListInterface {
public:
    virtual bool isEmpty() const = 0;
    virtual int getLength() const = 0;
    virtual bool insert(int newPosition, const T& newEntry) = 0;
    virtual bool remove(int position) = 0;
    virtual void clear() = 0;
    virtual T getEntry(int position) const = 0;
    virtual void setEntry(int position, const T& newEntry) = 0;
};

constexpr int MIN_ARRAY_SIZE = 64;

// **** PART 1 *****

template<typename T, int N>
class ArrayList final : public ListInterface<T> {
private:
    int itemCount;
    int maxItems;
    int items[N]{};
public:
    ArrayList() : itemCount(0), maxItems(N) {
        static_assert(N >= MIN_ARRAY_SIZE);
    }

    bool isEmpty() const {
        return itemCount == 0;
    }

    int getLength() const {
        return itemCount;
    };

    bool insert(int newPosition, const T& newEntry) {
        bool ableToInsert = (newPosition >= 1) &&
            (newPosition <= itemCount + 1) &&
            (itemCount < maxItems);
        if (ableToInsert) {
            // Make room for new entry by shifting all entries at
            // positions >= newPosition toward the end of the array
            // (no shift if newPosition == itemCount + 1)
            for (int pos = itemCount; pos >= newPosition; pos--)
                items[pos] = items[pos - 1];
            // Insert new entry
            items[newPosition - 1] = newEntry;
            itemCount++; // Increase count of entries
        } // end if
        return ableToInsert;
    }

    bool remove(int position) {
        bool ableToRemove = (position >= 1) && (position <= itemCount);
        if (ableToRemove) {
            // Remove entry by shifting all entries after the one at
            // position toward the beginning of the array
            // (no shift if position == itemCount)
            for (int fromIndex = position, toIndex = fromIndex - 1;
                fromIndex < itemCount; fromIndex++, toIndex++)
                items[toIndex] = items[fromIndex];
            itemCount--; // Decrease count of entries
        } // end if

        return ableToRemove;
    };

    void clear() {
        itemCount = 0;
    };

    T getEntry(int position) const {
        // Enforce precondition
        bool ableToGet = (position >= 1) && (position <= itemCount);
        if (ableToGet)
            return items[position - 1];
        else {
            string message = "getEntry() called with an empty list or ";
            message = message + "invalid position.";
            throw (std::invalid_argument(message));
        } // end if
    } // end getEntry

    void setEntry(int position, const T& newEntry) {
        // Enforce precondition
        bool ableToSet = (position >= 1) && (position <= itemCount);
        if (ableToSet)
            items[position - 1] = newEntry;
        else {
            string message = "setEntry() called with an empty list or ";
            message = message + "invalid position.";
            throw (std::invalid_argument(message));
        } // end if
    }
};

void testArrayList() {
    ArrayList<int, MIN_ARRAY_SIZE> array0;
    ArrayList<int, MIN_ARRAY_SIZE> array1;
    // 1
    assert(array0.isEmpty());
    // 2
    assert(array0.getLength() == 0);
    // 6
    assert(!array0.remove(0));
    // 8
    try {
        array0.getEntry(1);
        assert(false);
    }
    catch (std::invalid_argument& err) {}
    // 12
    try {
        array0.setEntry(1, 0);
        assert(false);
    }
    catch (std::invalid_argument& err) {}
    // 3
    array0.insert(1, 0);
    assert(array0.getLength() == 1);
    // 5
    assert(!array0.isEmpty());
    // 9
    assert(array0.getEntry(1) == 0);
    // 10
    array1.insert(1, 0);
    array1.insert(1, 1);
    assert(array0.getEntry(1) == array1.getEntry(2));
    // 11
    array0.insert(1, 1);
    array1.remove(1);
    assert(array0.getEntry(2) == array1.getEntry(1));
    // 4
    array0.remove(1);
    assert(array0.getLength() == 1);
    // 13
    array1.setEntry(1, 2);
    assert(array1.getEntry(1) == 2);

    // TODO: Add test for axiom 7.
}

template<typename T>
class Node {
private:
    T value;
    Node* next;

public:
    Node(T value) : value(value), next(nullptr) {}

    Node(T value, Node* next) : value(value), next(next) {}

    T getItem() const {
        return value;
    }

    Node* getNext() const {
        return next;
    }

    void setNext(Node* n) {
        next = n;
    }

    void setItem(const T& v) {
        value = v;
    }
};

template<class ItemType>
class LinkedList : public ListInterface<ItemType> {
private:
    // Pointer to first node in the chain (contains the first entry in the list)
    Node<ItemType>* headPtr;

    // Current count of list items
    int itemCount;

    // Locates a specified node in a linked list.
    // @pre position is the number of the desired node;
    // position >= 1 and position <= itemCount.
    // @post The node is found and a pointer to it is returned.
    // @param position The number of the node to locate.
    // @return A pointer to the node at the given position.
    Node<ItemType>* getNodeAt(int position) const {
        // Debugging check of precondition
        if (!((position >= 1) && (position <= itemCount))) {
            throw (std::invalid_argument("LinkedList error"));
        }
        else {
            // Count from the beginning of the chain
            Node<ItemType>* curPtr = headPtr;
            for (int skip = 1; skip < position; skip++)
                curPtr = curPtr->getNext();
            return curPtr;
        }
    }

public:
    LinkedList() : headPtr(nullptr), itemCount(0) {}

    ~LinkedList() {
        clear();
    };

    bool isEmpty() const {
        return itemCount == 0;
    }

    int getLength() const {
        return itemCount;
    }

    bool insert(int newPosition, const ItemType& newEntry) {
        bool ableToInsert = (newPosition >= 1) &&
            (newPosition <= itemCount + 1);
        if (ableToInsert) {
            // Create a new node containing the new entry
            Node<ItemType>* newNodePtr = new Node<ItemType>(newEntry);
            // Attach new node to chain
            if (newPosition == 1) {
                // Insert new node at beginning of chain
                newNodePtr->setNext(headPtr);
                headPtr = newNodePtr;
            }
            else {
                // Find node that will be before new node
                Node<ItemType>* prevPtr = getNodeAt(newPosition - 1);
                // Insert new node after node to which prevPtr points
                newNodePtr->setNext(prevPtr->getNext());
                prevPtr->setNext(newNodePtr);
            } // end if
            itemCount++; // Increase count of entries
        } // end if
        return ableToInsert;
    }

    bool remove(int position) {
        bool ableToRemove = (position >= 1) && (position <= itemCount);
        if (ableToRemove) {
            Node<ItemType>* curPtr = nullptr;
            if (position == 1) {
                // Remove the first node in the chain
                curPtr = headPtr; // Save pointer to node
                headPtr = headPtr->getNext();
            }
            else {
                // Find node that is before the one to delete
                Node<ItemType>* prevPtr = getNodeAt(position - 1);
                // Point to node to delete
                curPtr = prevPtr->getNext();
                // Disconnect indicated node from chain by connecting the
                // prior node with the one after
                prevPtr->setNext(curPtr->getNext());
            } // end if
            // Return node to system
            curPtr->setNext(nullptr);
            delete curPtr;
            curPtr = nullptr;
            itemCount--; // Decrease count of entries
        } // end if
        return ableToRemove;
    }

    void clear() {
        while (!isEmpty())
            remove(1);
    }

    /** @throw invalid_argument if position < 1 or position > getLength(). */
    ItemType getEntry(int position) const {
        // Enforce precondition
        bool ableToGet = (position >= 1) && (position <= itemCount);
        if (ableToGet) {
            Node<ItemType>* nodePtr = getNodeAt(position);
            return nodePtr->getItem();
        }
        else {
            string message = "getEntry() called with an empty list or ";
            message = message + "invalid position.";
            throw (std::invalid_argument(message));
        }
    }

    /** @throw invalid_argument if position < 1 or position > getLength(). */
    void setEntry(int position, const ItemType& newEntry) {
        Node<ItemType>* n = getNodeAt(position);
        n->setItem(newEntry);
    }
}; // end LinkedList

void testLinkedList() {
    LinkedList<int> list0;
    LinkedList<int> list1;
    // 1
    assert(list0.isEmpty());
    // 2
    assert(list0.getLength() == 0);
    // 6
    assert(!list0.remove(0));
    // 8
    try {
        list0.getEntry(1);
        assert(false);
    }
    catch (std::invalid_argument& err) {}
    // 12
    try {
        list0.setEntry(1, 0);
        assert(false);
    }
    catch (std::invalid_argument& err) {}
    // 3
    list0.insert(1, 0);
    assert(list0.getLength() == 1);
    // 5
    assert(!list0.isEmpty());
    // 9
    assert(list0.getEntry(1) == 0);
    // 10
    list1.insert(1, 0);
    list1.insert(1, 1);
    assert(list0.getEntry(1) == list1.getEntry(2));
    // 11
    list0.insert(1, 1);
    list1.remove(1);
    assert(list0.getEntry(2) == list1.getEntry(1));
    // 4
    list0.remove(1);
    assert(list0.getLength() == 1);
    // 13
    list1.setEntry(1, 2);
    assert(list1.getEntry(1) == 2);
}

// ***** PART 2 *****

template<typename ItemType>
void insertionSort(ListInterface<ItemType>& list) {
    for (int sorted = 1; sorted < list.getLength(); sorted++) {
        ItemType copy = list.getEntry(sorted + 1);
        int i = sorted;
        while (i > 0 and list.getEntry(i) > copy) {
            list.setEntry(i + 1, list.getEntry(i));
            i--;
        }
        list.setEntry(i + 1, copy);
    }
}

void fillRandom(LinkedList<int>& list, int n) {
    for (int i = 0; i < n; ++i) {
        int j = std::rand();
        list.insert(1, j);
    }
}

bool isSorted(const LinkedList<int>& list) {
    int n = list.getLength();
    if (n <= 1) {
        return true;
    }

    for (int i = 1; i < n; ++i) {
        if (list.getEntry(i) > list.getEntry(i + 1)) {
            return false;
        }
    }

    return true;
}

void testLinkedInsertionSort() {
    LinkedList<int> list0;
    insertionSort(list0);
    assert(list0.isEmpty());

    LinkedList<int> list1;
    list1.insert(1, 10);
    insertionSort(list1);
    assert(list1.getLength() == 1);

    LinkedList<int> list2;
    list2.insert(1, 10);
    list2.insert(1, 20);
    insertionSort(list2);
    assert(list2.getEntry(1) == 10);
    assert(list2.getEntry(2) == 20);

    LinkedList<int> list3;
    list3.insert(1, 10);
    list3.insert(1, 20);
    list3.insert(1, 30);

    insertionSort(list3);

    assert(list3.getEntry(1) == 10);
    assert(list3.getEntry(2) == 20);
    assert(list3.getEntry(3) == 30);

    LinkedList<int> listRandom;
    fillRandom(listRandom, 32);
    insertionSort(listRandom);
    assert(isSorted(listRandom));
}

void testArrayInsertionSort() {
    ArrayList<int, MIN_ARRAY_SIZE> list0;
    insertionSort(list0);
    assert(list0.isEmpty());

    ArrayList<int, MIN_ARRAY_SIZE> list1;
    list1.insert(1, 10);
    insertionSort(list1);
    assert(list1.getLength() == 1);

    ArrayList<int, MIN_ARRAY_SIZE> list2;
    list2.insert(1, 10);
    list2.insert(1, 20);
    insertionSort(list2);
    assert(list2.getEntry(1) == 10);
    assert(list2.getEntry(2) == 20);

    ArrayList<int, MIN_ARRAY_SIZE> list3;
    list3.insert(1, 10);
    list3.insert(1, 20);
    list3.insert(1, 30);

    insertionSort(list3);

    assert(list3.getEntry(1) == 10);
    assert(list3.getEntry(2) == 20);
    assert(list3.getEntry(3) == 30);

    LinkedList<int> listRandom;
    fillRandom(listRandom, 32);
    insertionSort(listRandom);
    assert(isSorted(listRandom));
}

// ***** PART 3 ****

template<typename SongType>
class PlaylistInterface : public ListInterface<SongType> {
private:
    int currentSongPosition;
    enum MODE_ENUM mode;
public:
    const enum MODE_ENUM {
        LOOP,
        RANDOM,
        PLAY_ONCE,
    };

    // accepts a playlist position (either an existing position or length+1) and song to add
    // adds given song in the given position, shifting other songs forward in the list to accommodate
    // throws an error if an invalid position is given
    virtual void addSong(int position, const SongType& newSong);

    // accepts the existing position of a song
    // removes the song in the given position, shifting other songs back in the list to fill the space
    // throws an error if an invalid position is given
    virtual void removeSong(int position);

    // accepts an enum value from the public modes enum, and changes mode to the given setting
    virtual void setMode(enum MODE_ENUM newMode);

    // updates currentSongPosition based on the current mode and returns the song at that position
    virtual SongType nextSong();

    // updates currentSongPosition to the previously played song and returns the song at that position
    // accounts for random mode when finding previous song
    virtual SongType previousSong();

    // returns the song to be played next based on the current mode, but does not update currentSongPosition
    virtual SongType peekNextSong();

    // accepts the position of a song in the playlist, and another existing position to move it to
    // moves the song at startPosition to endPosition, shifting other songs towards the vacated position
    // throws an error if an invalid position is given
    virtual void moveSong(int startPosition, int endPosition);

    // sorts playlist alphabetically by title
    virtual void sortByTitle();

    // sorts playlist alphabetically by artist
    virtual void sortByArtist();

    // sorts playlist alphabetically by album
    virtual void sortByAlbum();

    // sorts playlist alphabetically by genre
    virtual void sortByGenre();
};

// ***** PART 4 ****

template<typename T>
class SmartNode {
private:
    T value;
    std::shared_ptr<SmartNode> next;

public:
    SmartNode(T value) : value(value), next(nullptr) {}

    SmartNode(T value, std::shared_ptr<SmartNode> next) : value(value), next(next) {}

    T getItem() const {
        return value;
    }

    std::shared_ptr<SmartNode> getNext() const {
        return next;
    }

    void setNext(std::shared_ptr<SmartNode> n) {
        next = n;
    }

    void setItem(const T& v) {
        value = v;
    }
};

template<class ItemType>
class SmartLinkedList : public ListInterface<ItemType> {
private:
    std::shared_ptr<SmartNode<ItemType>> headPtr;
    int itemCount;

    std::shared_ptr<SmartNode<ItemType>> getNodeAt(int position) const {
        if (!(position >= 1 and position <= itemCount)) {
            throw (std::invalid_argument("LinkedList error"));
        }
        else {
            std::shared_ptr<SmartNode<ItemType>> curPtr = headPtr;
            for (int skip = 1; skip < position; skip++) {
                curPtr = curPtr->getNext();
            }
            return curPtr;
        }
    }
public:
    SmartLinkedList() : headPtr(nullptr), itemCount(0) {}

    ~SmartLinkedList() {
        clear();
    };

    bool isEmpty() const {
        return itemCount == 0;
    }

    int getLength() const {
        return itemCount;
    }

    bool insert(int newPosition, const ItemType& newEntry) {
        bool ableToInsert = (newPosition >= 1) && (newPosition <= itemCount + 1);
        if (ableToInsert) {
            // create new node
            auto newNodePtr = std::shared_ptr(SmartNode<ItemType>(newEntry));
            // attach to chain
            if (newPosition == 1) {
                newNodePtr->setNext(headPtr);
                headPtr = newNodePtr;
            }
            else {
                std::shared_ptr<SmartNode<ItemType>> prevPtr = getNodeAt(newPosition - 1);
                newNodePtr->setNext(prevPtr->getNext);
                prevPtr->setNext(newNodePtr);
            } // end if
            itemCount++;
        } // end if
        return ableToInsert;
    }

    bool remove(int position) {
        bool ableToRemove = (position >= 1) and (position <= itemCount);
        if (ableToRemove) {
            if (position == 1) {
                headPtr = headPtr->getNext();
            }
            else {
                std::shared_ptr<SmartNode<ItemType>> curPtr = getNodeAt(position);
                std::shared_ptr<SmartNode<ItemType>> prevPtr = getNodeAt(position - 1);
                prevPtr->setNext(curPtr->getNext());
            } // end if
            itemCount--;
        } // end if
        return ableToRemove;
    }

    void clear() {
        headPtr = nullptr;
    }

    ItemType getEntry(int position) const {
        bool ableToGet = (position >= 1) and (position <= itemCount);
        if (ableToGet) {
            std::shared_ptr<Node<ItemType>> nodePtr = getNodeAt(position);
            return nodePtr->getItem();
        }
        else {
            string message = "getEntry() called with an empty list or invalid position.";
            throw (std::invalid_argument(message));
        }
    }

    void setEntry(int position, const ItemType& newEntry) {
        std::shared_ptr<Node<ItemType>> n = getNodeAt(position);
        n->setItem(newEntry);
    }
}; 

void testSmartLinkedList() {
    LinkedList<int> list0;
    LinkedList<int> list1;
    // 1
    assert(list0.isEmpty());
    // 2
    assert(list0.getLength() == 0);
    // 6
    assert(!list0.remove(0));
    // 8
    try {
        list0.getEntry(1);
        assert(false);
    }
    catch (std::invalid_argument& err) {}
    // 12
    try {
        list0.setEntry(1, 0);
       assert(false);
    }
    catch (std::invalid_argument& err) {}
    // 3
    list0.insert(1, 0);
    assert(list0.getLength() == 1);
    // 5
    assert(!list0.isEmpty());
    // 9
    assert(list0.getEntry(1) == 0);
    // 10
    list1.insert(1, 0);
    list1.insert(1, 1);
    assert(list0.getEntry(1) == list1.getEntry(2));
    // 11
    list0.insert(1, 1);
    list1.remove(1);
    assert(list0.getEntry(2) == list1.getEntry(1));
    // 4
    list0.remove(1);
    assert(list0.getLength() == 1);
    // 13
    list1.setEntry(1, 2);
    assert(list1.getEntry(1) == 2);
}

int main() {
    std::srand(0);

    testArrayList();
    testLinkedList();
    testLinkedInsertionSort();
    testArrayInsertionSort();
    testSmartLinkedList();
    return 0;
}