import sys
import string

args = sys.argv[1:]

#######################################
# CONSTANTS
#######################################

DIGITS = '0123456789'
LETTERS = string.ascii_letters
STR_LETTERS = string.printable
LETTERS_DIGITS = LETTERS + DIGITS + "_"

#######################################
# ERRORS
#######################################


class Error:

    def __init__(self, pos_start, pos_end, error_name, details):
        self.pos_start = pos_start
        self.pos_end = pos_end
        self.error_name = error_name
        self.details = details

    def as_string(self):
        result = f'{self.error_name}: {self.details}\n'
        return result


class IllegalCharError(Error):

    def __init__(self, pos_start, pos_end, details):
        super().__init__(pos_start, pos_end, 'Illegal Character', details)


#######################################
# POSITION
#######################################


class Position:

    def __init__(self, idx, ln, col, ftxt):
        self.idx = idx
        self.ln = ln
        self.col = col
        self.ftxt = ftxt

    def advance(self, current_char):
        self.idx += 1
        self.col += 1

        if current_char == '\n':
            self.ln += 1
            self.col = 0

        return self

    def copy(self):
        return Position(self.idx, self.ln, self.col, self.ftxt)

    def __repr__(self) -> str:
        return str(self.idx)


#######################################
# ENV
#######################################

vars = {}
funcs = {}

#######################################
# TOKENS
#######################################

TT_INT = 'INT'
TT_FLOAT = 'FLOAT'
TT_STRING = 'STRING'
TT_CHAR = 'CHAR'
TT_BOOL = 'BOOL'
TT_IDENTIFIER = 'IDENTIFIER'
TT_KEYWORD = 'KEYWORD'
TT_PLUS = 'PLUS'
TT_MINUS = 'MINUS'
TT_MUL = 'MUL'
TT_DIV = 'DIV'
TT_POW = 'POW'
TT_ASSIGN = 'ASSIGN'
TT_LPAREN = 'LPAREN'
TT_RPAREN = 'RPAREN'
TT_EE = 'EE'
TT_NE = 'NE'
TT_LT = 'LT'
TT_GT = 'GT'
TT_LTE = 'LTE'
TT_GTE = 'GTE'
TT_TYPE = 'TYPE'

KEYWORDS = ["goto", "read", "write", "while", "if", "for", "func"]

TYPES = ["int", "char", "str", "bool", "float"]


class Token:

    def __init__(self, type_, start, end=None, value=None):
        self.type = type_
        self.value = value
        self.pos_start = start
        self.pos_end = end if end != None else start + 1

    def __repr__(self):
        if self.value: return f'{self.type}: {self.value}'
        return f'{self.type}'


#######################################
# LEXER
#######################################


class Lexer:

    def __init__(self, text):
        self.text = text
        self.pos = Position(-1, 0, -1, text)
        self.current_char = None
        self.advance()

    def advance(self):
        self.pos.advance(self.current_char)
        self.current_char = self.text[self.pos.idx] if self.pos.idx < len(
            self.text) else None

    def make_tokens(self):
        tokens = []

        while self.current_char != None:
            if self.current_char in ' \t':
                self.advance()
            elif self.current_char in DIGITS:
                tokens.append(self.make_number())
            elif self.current_char in LETTERS:
                token, error = self.make_string()
                if error == None:
                    tokens.append(token)
                else:
                    return [], error
            elif self.current_char == '+':
                tokens.append(Token(TT_PLUS, self.pos.idx))
                self.advance()
            elif self.current_char == '-':
                tokens.append(Token(TT_MINUS, self.pos.idx))
                self.advance()
            elif self.current_char == '*':
                tokens.append(Token(TT_MUL, self.pos.idx))
                self.advance()
            elif self.current_char == '/':
                tokens.append(Token(TT_DIV, self.pos.idx))
                self.advance()
            elif self.current_char == '^':
                tokens.append(Token(TT_POW, self.pos.idx))
                self.advance()
            elif self.current_char == '(':
                tokens.append(Token(TT_LPAREN, self.pos.idx))
                self.advance()
            elif self.current_char == ')':
                tokens.append(Token(TT_RPAREN, self.pos.idx))
                self.advance()
            elif self.current_char in ('<', '>', '=', '!'):
                token, error = self.make_comp()
                if error == None:
                    tokens.append(token)
                else:
                    return [], error
            elif self.current_char in ("'", '"'):
                self.advance()
                token, error = self.make_string(True)
                if error == None:
                    tokens.append(token)
                else:
                    return [], error
            else:
                pos_start = self.pos.copy()
                char = self.current_char
                self.advance()
                return [], IllegalCharError(pos_start, self.pos,
                                            "'" + char + "'")

        return tokens, None

    def make_number(self):
        num_str = ''
        dot_count = 0
        start = self.pos.copy()

        while self.current_char != None and self.current_char in DIGITS + '.':
            if self.current_char == '.':
                if dot_count == 1: break
                dot_count += 1
                num_str += '.'
            else:
                num_str += self.current_char
            self.advance()

        if dot_count == 0:
            return Token(TT_INT, start.idx, self.pos.idx, int(num_str))
        else:
            return Token(TT_FLOAT, start.idx, self.pos.idx, float(num_str))

    def make_string(self, is_str=False):
        string = ''
        pos_start = self.pos.copy()
        escape_character = False

        escape_characters = {'n': '\n', 't': '\t'}

        while self.current_char is not None and (
                self.current_char
                in (STR_LETTERS if is_str else LETTERS_DIGITS) and
            (self.current_char != '"' or escape_character)):
            if escape_character:
                string += escape_characters.get(self.current_char,
                                                self.current_char)
            else:
                if self.current_char == '\\':
                    escape_character = True
                else:
                    string += self.current_char
            self.advance()
            escape_character = False

        past_char = self.current_char
        self.advance()
        if not is_str:
            if string in KEYWORDS:
                return Token(TT_KEYWORD, pos_start, self.pos, string), None
            elif string in TYPES:
                return Token(TT_TYPE, pos_start, self.pos, string), None
        else:
            if len(string) > 1:
                if past_char in ("'", '"'):
                    return Token(TT_STRING, pos_start, self.pos, string), None
                else:
                    return None, Error(pos_start, self.pos, "String error",
                                       f"You forgot quotes")
            elif is_str:
                if past_char in ("'", '"'):
                    return Token(TT_CHAR, pos_start, self.pos, string), None
                else:
                    return None, Error(pos_start, self.pos, "String error",
                                       f"You forgot quotes")
            else:
                return None, Error(pos_start, self.pos, "NameError",
                                   f"{string} is not defined.")

    def make_comp(self):
        pos_start = self.pos.copy()

        if self.current_char == ">":
            self.advance()
            if self.current_char == "=":
                self.advance()
                return Token(TT_GTE, pos_start, self.pos), None
            else:
                return Token(TT_GT, pos_start, self.pos), None
        elif self.current_char == "<":
            self.advance()
            if self.current_char == "=":
                self.advance()
                return Token(TT_LTE, pos_start, self.pos), None
            else:
                return Token(TT_LT, pos_start, self.pos), None
        elif self.current_char == "=":
            self.advance()
            if self.current_char == "=":
                self.advance()
                return Token(TT_EE, pos_start, self.pos), None
            else:
                return Token(TT_ASSIGN, pos_start, self.pos), None
        elif self.current_char == "!":
            self.advance()
            if self.current_char == "=":
                self.advance()
                return Token(TT_EE, pos_start, self.pos), None
            else:
                return None, Error
        return None, None


#######################################
# PARSER
#######################################


class Parser:

    def __init__(self, tokens):
        self.tokens = tokens
        self.current_token = None
        self.token_index = -1
        self.advance()

    def advance(self):
        self.token_index += 1
        if self.token_index < len(self.tokens):
            self.current_token = self.tokens[self.token_index]
        return self.current_token

    def factor(self):
        token = self.current_token
        if token.type == TT_INT or token.type == TT_FLOAT:
            self.advance()
            return token
            self.advance()
            return token
        elif token.type == TT_LPAREN:
            self.advance()
            node = self.expr()
            if self.current_token.type == TT_RPAREN:
                self.advance()
                return node
            else:
                raise Exception("Expected ')'")
        else:
            raise Exception("Invalid Syntax")

    def expon(self):
        node = self.factor()
        while self.current_token.type == TT_POW:
            token = self.current_token
            if token.type == TT_POW:
                self.advance()
            node = [node, token, self.factor()]
        return node

    def term(self) -> list:
        node = self.expon()
        while self.current_token.type in (TT_MUL, TT_DIV):
            token = self.current_token
            if token.type == TT_MUL:
                self.advance()
            else:
                self.advance()
            node = [node, token, self.expon()]
        return node

    def expr(self) -> list:
        node = self.term()
        while self.current_token.type in (TT_PLUS, TT_MINUS):
            token = self.current_token
            if token.type == TT_PLUS:
                self.advance()
            else:
                self.advance()
            node = [node, token, self.term()]
        return node

    def make_tree(self) -> list:
        if self.tokens == []:
            return []
        else:
            return list(self.expr())


#######################################
# Interpreter
#######################################


class Interpreter:

    def __init__(self, tokens, isparen=False):
        self.tokens = tokens
        self.isparen = isparen

    def parse(self):
        tokens = self.tokens
        resualt = 0
        number = None
        op = None
        isneg = False
        onenum = True
        if tokens == []:
            return 0, None
        elif type(tokens) == None:
            return 0, None
        elif type(tokens) == Token:
            tokens = [tokens]
        for idx, item in enumerate(tokens):
            if type(item) == list:
                toadd, _ = Parser(item).parse()
                if _ != None and toadd == None:
                    return None, _
                elif toadd == None:
                    continue
                elif '.' in str(toadd):
                    item = Token(TT_FLOAT, idx, idx + len(str(toadd)), toadd)
                else:
                    item = Token(TT_INT, idx, idx + len(str(toadd)), toadd)
            if item.type == TT_LPAREN:
                toadd, _ = Parser(tokens[idx + 1:], True).parse()
                if _ != None and toadd == None:
                    return None, _
                elif toadd == None:
                    continue
                elif '.' in str(toadd):
                    item = Token(TT_FLOAT, idx, idx + len(str(toadd)), toadd)
                else:
                    item = Token(TT_INT, idx, idx + len(str(toadd)), toadd)
            else:
                if item.type in (TT_INT, TT_FLOAT):
                    if number == None:
                        if isneg:
                            number = Token(item.type, item.pos_start,
                                           item.pos_end, -item.value)
                            isneg = False
                        else:
                            number = item
                    elif number != None and op != None:
                        if op.type == TT_PLUS:
                            resualt = number.value + item.value
                        elif op.type == TT_MINUS:
                            resualt = number.value - item.value
                        elif op.type == TT_MUL:
                            resualt = number.value * item.value
                        elif op.type == TT_DIV:
                            resualt = number.value / item.value
                        elif op.type == TT_POW:
                            resualt = number.value**item.value
                        number = None
                        op = None
                    else:
                        return None, Error(
                            idx, idx, "Can't have two numbers",
                            "You can't have two numbers, BUDDY"
                        )  #TODO: write an actual error before we put this in prod
                elif item.type == TT_PLUS:
                    op = item
                    onenum = False
                elif item.type == TT_MINUS:
                    if number != None:
                        op = item
                        onenum = False
                    else:
                        isneg = True
                elif item.type == TT_MUL:
                    op = item
                    onenum = False
                elif item.type == TT_DIV:
                    op = item
                    onenum = False
                elif item.type == TT_POW:
                    op = item
                    onenum = False
            if self.isparen == True and idx == len(tokens) - 1:
                if item.type != TT_RPAREN:
                    return None, Error(
                        idx, idx, "You forgor )", "put ), BUDDY"
                    )  #TODO: write an actual error before we put this in prod
            if item.type == TT_RPAREN:
                if resualt == None:
                    return 0, None
                else:
                    return number.value if onenum or item == None else resualt, None
        if resualt == None:
            return 0, None
        else:
            return number.value if onenum or item == None else resualt, None


#######################################
# RUN
#######################################


def run(text):
    if text == "":
        return "", None
    lexer = Lexer(text)
    tokens, error = lexer.make_tokens()
    """if error:
        return None, error

    
    treemake = Treemaker(list(tokens))
    tree = treemake.make_tree()

    parsed, error = Parser(tree).parse()"""

    return tokens, error


if len(args) == 0:
    while True:
        text = input('basic > ')
        if text == "exit()":
            break
        elif "exit" in text:
            print("Use exit() to exit")
            continue
        tokens, error = run(text)
        if error != None:
            print(error.as_string())
        else:
            print(tokens)
elif len(args) == 1:
    fn = args[0]
    with open(fn, "r") as file:
        for line in file.read().splitlines():
            tokens, error = run(line)
            if error != None:
                print(error.as_string())
                break
            else:
                print(tokens)
else:
    print("Invalid Args")
