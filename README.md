# Compiler Theory and Practice Course Assignment 

# Author
**Matthias Bartolo 0436103L**

## Preview:
<p align='center'>
  <img src="https://github.com/mbar0075/Compiler-Theory-and-Practice/assets/103250564/6ae4adf0-6cd7-47a6-86ed-836285192926" style="display: block; margin: 0 auto; width: 90%; height: auto;">
</p>

## Description of Task:
The task at hand involved the development of a higher-level and more intuitive programming interface for PixArDis, a startup company specializing in programmable pixel art displays. The objective was to address the mixed reviews received for their first display model, PAD2000, which showcased excellent hardware capabilities utilizing energy-saving LED technology. However, negative feedback pointed out shortcomings in the programming interface.

To overcome these concerns, a task was assigned to develop a new imperative-style programming language called PixArLang. The goal was to create a user-friendly interface that could be used by PixArDis clients to program the PAD2000 display. In the interim, the R&D team at PixArDis had developed an online simulator to mimic the behavior of the PAD2000 hardware, facilitating the development of PixArLang.

The completed task involved the implementation of a lexer, parser, semantic analyzer, and code generator targeting PixIR, an assembly-like language utilized by the PAD2000. The task consisted of five major components:

- **Task 1 - Table-driven lexer**
- **Task 2 - Hand-crafted LL(k) parser**
- **Task 3 - AST XML Generation Pass**
- **Task 4 - Semantic Analysis Pass**
- **Task 5 - PixIR Code Generation Pass**

Throughout the completion of the task, a lexer was created to scan the input code and identify tokens with their associated attributes. These tokens were then used by the parser to construct an AST. Visitor classes played a crucial role in generating XML representations, performing semantic analysis, and generating executable code in PixIR.

The successful completion of this task has significantly contributed to the development of PixArLang, offering PixArDis clients an improved, user-friendly programming interface for the PAD2000 pixel art displays.

## Video:


https://github.com/mbar0075/Compiler-Theory-and-Practice/assets/103250564/9f0622fb-c472-40ac-8a03-0a5d20121b10



## The PixArLang programming language

The following rules describe the syntax of TinyLang in EBNF. Each rule has three parts: a left
hand side (LHS), a right-hand side (RHS) and the ‘::=’ symbol separating these two sides. The
LHS names the EBNF rule whereas the RHS provides a description of this name. Note that the
RHS uses four control forms namely sequence, choice, option and repetition. In a sequence order
is important and items appear left-to-right. The stroke symbol ( . . . | . . .) is used to denote choice
between alternatives. One item is chosen from this list; order is not important. Optional items are
enclosed in square brackets ([ . . . ]) indicating that the item can either be included or discarded.
Repeatable items are enclosed in curly brackets ({ . . . }); the items within can be repeated zero or
more times. For example, a Block consists of zero or more Statement enclosed in curly brackets.

```ebnf
<Letter> ::= [A-Za-z]
<Digit> ::= [0-9]
<Hex> ::= [A-Fa-f] | <Digit>
<Type> ::= 'float' | 'int' | 'bool' | 'colour'
<BooleanLiteral> ::= 'true' | 'false'
<IntegerLiteral> ::= <Digit> { <Digit> }
<FloatLiteral> ::= <Digit> { <Digit> } '.' <Digit> { <Digit> }
<ColourLiteral> ::= '#' <Hex> <Hex> <Hex> <Hex> <Hex> <Hex>
<PadWidth> ::= '__width'
<PadHeight> ::= '__height'
<PadRead> ::= '__read' <Expr> ',' <Expr>
<PadRandI> ::= '__randi' <Expr>
<Literal> ::= <BooleanLiteral>
            | <IntegerLiteral>
            | <FloatLiteral>
            | <CharLiteral>
            | <ColourLiteral>
            | <PadWidth>
            | <PadHeight>
            | <PadRead>
<Identifier> ::= (<Letter> | '_') { <Letter> | <Digit> | '_' }
<MultiplicativeOp> ::= '*' | '/' | 'and'
<AdditiveOp> ::= '+' | '-' | 'or'
<RelationalOp> ::= '<' | '>' | '==' | '!=' | '<=' | '>='
<ActualParams> ::= <Expression> { ',' <Expression> }
<FunctionCall> ::= <Identifier> '(' [ <ActualParams> ] ')'
<SubExpression> ::= '(' <Expression> ')'
<Unary> ::= ( '-' | 'not' ) <Expression>
<Factor> ::= <Literal>
            | <Identifier>
            | <FunctionCall>
            | <SubExpression>
            | <Unary>
            | <PadRandI>
            | <PadWidth>
            | <PadHeight>
            | <PadRead>
<Term> ::= <Factor> { <MultiplicativeOp> <Factor> }
<SimpleExpr> ::= <Term> { <AdditiveOp> <Term> }
<Expression> ::= <SimpleExpr> { <RelationalOp> <SimpleExpr> }
<Assignment> ::= <Identifier> '=' <Expression>
<VariableDecl> ::= 'let' <Identifier> ':' <Type> '=' <Expression>
<PrintStatement> ::= '__print' <Expression>
<DelayStatement> ::= '__delay' <Expr>
<PixelStatement> ::= '__pixelr' <Expr> ',' <Expr> ',' <Expr> ',' <Expr> ',' <Expr>
                   | '__pixel' <Expr> ',' <Expr> ',' <Expr>
<RtrnStatement> ::= 'return' <Expression>
<IfStatement> ::= 'if' '(' <Expression> ')' <Block> [ 'else' <Block> ]
<ForStatement> ::= 'for' '(' [ <VariableDecl> ] ';' <Expression> ';' [ <Assignment> ] ')' <Block>
<WhileStatement> ::= 'while' '(' <Expression> ')' <Block>
<FormalParam> ::= <Identifier> ':' <Type>
<FormalParams> ::= <FormalParam> { ',' <FormalParam> }
<FunctionDecl> ::= 'fun' <Identifier> '(' [ <FormalParams> ] ')' '->' <Type> <Block>
<ClearStatement> ::= '__clear' <ColourLiteral>
<Statement> ::= <VariableDecl> ';'
                | <Assignment> ';'
                | <PrintStatement> ';'
                | <DelayStatement> ';'
                | <PixelStatement> ';'
                | <ClearStatement> ';'
                | <IfStatement>
                | <ForStatement>
                | <WhileStatement>
                | <RtrnStatement> ';'
                | <FunctionDecl>
                | <Block>
<Block> ::= '{' { <Statement> } '}'
<Program> ::= { <Statement> }
```

## Deterministic Finite State Automata (DFSA)
The Lexer implementation utilises regular expressions as a transition system, in order to tokenise the
input program to valid tokens, which will later be passed on to the Parser. Furthermore, to facilitate this,
a deterministic finite state automata (DFSA) was used in order implement such regular expressions.
The DFSA utilised can be seen pictorially below:

<p align='center'>
  <img src="Report Diagrams/LexerDFSADiagram.png" style="display: block; margin: 0 auto; width: 70%; height: auto;">
</p>

## Abstract Syntax Tree
The task at hand involved coding the Abstract Syntax Tree (AST) as a tree structure and storing it as a vector of shared pointers that pointed to the child nodes. A class hierarchy was used to represent each node in the AST, accommodating operators, types, identifier names, and other relevant elements. This design choice aimed to achieve efficient storage and traversal of the AST using the visitor design pattern.

Shared pointers were employed for ease of deletion and to prevent memory leaks. It was ensured that deleting the starting ASTNode would lead to the automatic deletion of the entire tree structure, thereby maintaining proper memory management.

By implementing the AST in this manner, the task aimed to achieve a robust and efficient representation of the program's structure. This would facilitate subsequent analysis and code generation processes, enabling effective interpretation and execution of the PixArLang programming language.

<p align='center'>
  <img src="Report Diagrams/ASTNodesHierarchyDiagram.png" style="display: block; margin: 0 auto; width: 100%; height: auto;">
</p>

## Showcasing PixArLang Syntax
```
fun DrawBow1()-> int{
    __clear #ffffff;
    let col1:colour=__randi #ffffff;
    let col2:colour=__randi #ffffff;
    for(let x:int=0;x<=__width;x=x+1){
        for(let y:int=0;y<=x;y=y+1){
            __pixel x,y,col1;
            __pixel x,__height-y,col2;
        }
    }
    return 3;
}

fun DrawBow2()-> int{
    __clear #ffffff;
    let col1:colour=__randi #ffffff;
    let col2:colour=__randi #ffffff;
    for(let x:int=__width;x>=0;x=x-1){
        for(let y:int=x-1;y>=0;y=y-1){
            __pixel x,y,col1;
            __pixel x,__height-y,col2;
        }
    }
    return 3;
}

fun DrawStairs() -> int{
    for(let a:int=0; a<__width; a=a+3){
        let clr:colour = __randi #ffffff;
        for(let x:int=a; x<__width; x=x+1){
            __pixelr x,x,__height-x,__height-x,clr;
        }
    }
    return 0;
}

fun LoopBow(iterations:int)->int{
let xCount:int=0;
    while(xCount<iterations){
        let random1:int=__randi 3;
        if(random1==1){
            let bow:int=DrawBow1();
        }
        else{
            if(random1==2){
                let stairs:int=DrawStairs();
            }
            else{
                let bow:int=DrawBow2();
            }
        }
        xCount=xCount+1;
        __delay 100;
    }
    return 0;
}

let program:int=LoopBow(100);
__print 0;
```
</br>The presented illustration showcases a syntax tree that represents an assignment:
<p align='center'>
  <img src="Report Diagrams/SyntaxTreeExample.png" style="display: block; margin: 0 auto; width: 30%; height: auto;">
</p>

## Deliverables:
The repository includes:<br />
1. **Compiler Theory** - Directory which holds implementation of the PixArDis Compiler.
2. **Report Diagrams** - Directory which holds the diagrams constructed through draw.io, to be used in the report.
3. **Sample Programs** - Directory which holds the sample PixArLang programs, which can be fed to the constructed PixArDis Compiler.
4. **VM Box Used to Run Generated Code** - Directory which holds the Virtual Machine utilised to run the PixIR code generated from the Compiler.
5. **Video** - Directory which holds the video demonstration of the constructed Compiler.
6. **report.pdf** - Documentation for the PixArDis Compiler.

## Additional Info:
Running the Virtual Machine, requires the installation of the node.js package module.<br />
