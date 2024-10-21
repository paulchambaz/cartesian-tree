#include "Node.h"

namespace complex {

Node::~Node() {
  delete left;
  delete right;
}

} // namespace complex
