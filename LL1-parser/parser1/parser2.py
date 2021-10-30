from queue import LifoQueue

from anytree import Node, RenderTree

from parser1.functions import parse_bnf


class Stack(LifoQueue):

    def peek(self):

        if len(self.queue) == 0:
            return None
        return self.queue[len(self.queue) - 1]


def parse(grammar, words):
    """Parse function"""
    grammar = parse_bnf(grammar)
    table, ambiguous = grammar.parsing_table(is_clean=True)
    if ambiguous:
        raise Warning("Es ambiguo")

    error_list = []

    words.append("$")
    word = words.pop(0)
    stack = Stack()

    stack.put(("$", None))
    root = Node(grammar.start)
    stack.put((grammar.start, root))

    top_stack = stack.peek()
    while True:
        print(f"Current_word:{word},  Stack:{stack.queue}")
        if top_stack[0] == "$" and word == "$":
            if not error_list:
                return True, root, None
            else:
                return False, root, error_list

        if grammar.is_terminal(top_stack[0]):
            if top_stack[0] == word:
                print(f"Consume input: {word}")
                stack.get()
                word = words.pop(0)
            else:
                error_list.append(f"Expected {top_stack[0]}")
                while word != top_stack[0]:
                    if word == "$":
                        return False, root, error_list
                    word = words.pop(0)
        else:
            rule = table.get((top_stack[0], word))
            stack.get()
            if rule:
                print(f"Rule: {rule}")
                symbols = rule.body[::-1]
                for symbol in symbols:
                    node = Node(symbol, parent=top_stack[1])
                    if symbol != "&":
                        stack.put((symbol, node))
            else:
                error_list.append(f"Unexpected character:{word}. Expected: {grammar.first(top_stack[0])}")
                follow = grammar.follow(top_stack[0]) + ["$"]
                print(f"Error! Sync set: {follow}")
                while word not in follow:
                    print(f"Skipped: {word}")
                    word = words.pop(0)
        top_stack = stack.peek()


def parse_tree(grammar, words):
    result, tree, errors = parse(grammar, words)
    if not result:
        return
    for pre, fill, node in RenderTree(tree):
        print(f"{pre}{node.name}")
