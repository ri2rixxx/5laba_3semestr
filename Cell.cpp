#include "Cell.h"

Cell::Cell() : isOpen(false), isFlagged(false) {}

Cell::~Cell() {}

bool Cell::getIsOpen() const { 
    return isOpen; 
}

bool Cell::getIsFlagged() const { 
    return isFlagged; 
}

void Cell::setOpen(bool open) { 
    isOpen = open; 
}

void Cell::toggleFlag() { 
    if (!isOpen) {
        isFlagged = !isFlagged; 
    }
}
