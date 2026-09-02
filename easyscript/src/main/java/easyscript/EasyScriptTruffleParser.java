package easyscript;

import org.antlr.v4.runtime.BailErrorStrategy;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.TerminalNode;

import java.io.IOException;
import java.io.Reader;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public final class EasyScriptTruffleParser {
  public static List<EasyScriptStmtNode> parse(Reader program) throws IOException {
    var lexer = new EasyScriptLexer(CharStreams.fromReader(program));
    lexer.removeErrorListeners();
    var parser = new EasyScriptParser(new CommonTokenStream(lexer));
    parser.removeErrorListeners();
    parser.setErrorHandler(new BailErrorStrategy());
    return parseStmtList(parser.start().stmt());
  }

  private static List<EasyScriptStmtNode> parseStmtList(List<EasyScriptParser.StmtContext> stmts) {
    return stmts.stream()
        .flatMap(stmt -> stmt instanceof EasyScriptParser.ExprStmtContext
            ? Stream.of(parseExprStmt((EasyScriptParser.ExprStmtContext) stmt))
            : parseDeclStmt((EasyScriptParser.DeclStmtContext) stmt))
        .collect(Collectors.toList());
  }

  private static ExprStmtNode parseExprStmt(EasyScriptParser.ExprStmtContext exprStmt) {
    return new ExprStmtNode(parseExpr1(exprStmt.expr1()));
  }

  private static Stream<EasyScriptStmtNode> parseDeclStmt(EasyScriptParser.DeclStmtContext declStmt) {
    DeclarationKind declarationKind = DeclarationKind.fromToken(declStmt.kind.getText());
    return declStmt.binding()
        .stream()
        .map(binding -> {
          String variableId = binding.ID().getText();
          var bindingExpr = binding.expr1();
          EasyScriptExprNode initializerExpr;
          if (bindingExpr == null) {
            if (declarationKind == DeclarationKind.CONST) {
              throw new EasyScriptException("Missing initializer in const declaration '" + variableId + "'");
            }
            initializerExpr = new UndefinedLiteralExprNode();
          } else {
            initializerExpr = parseExpr1(bindingExpr);
          }
          return GlobalVarDeclStmtNodeGen.create(initializerExpr, variableId, declarationKind);
        });
  }

  private static EasyScriptExprNode parseExpr1(EasyScriptParser.Expr1Context expr1) {
    return expr1 instanceof EasyScriptParser.AssignmentExpr1Context
        ? parseAssignmentExpr((EasyScriptParser.AssignmentExpr1Context) expr1)
        : parseExpr2(((EasyScriptParser.PrecedenceTwoExpr1Context) expr1).expr2());
  }

  private static GlobalVarAssignmentExprNode parseAssignmentExpr(
      EasyScriptParser.AssignmentExpr1Context assignmentExpr) {
    String variableId = assignmentExpr.ID().getText();
    return GlobalVarAssignmentExprNodeGen.create(parseExpr1(assignmentExpr.expr1()), variableId);
  }

  private static EasyScriptExprNode parseExpr2(EasyScriptParser.Expr2Context expr2) {
    return expr2 instanceof EasyScriptParser.AddExpr2Context
        ? parseAdditionExpr((EasyScriptParser.AddExpr2Context) expr2)
        : parseExpr3(((EasyScriptParser.PrecedenceThreeExpr2Context) expr2).expr3());
  }

  private static AdditionExprNode parseAdditionExpr(EasyScriptParser.AddExpr2Context addExpr) {
    return AdditionExprNodeGen.create(
        parseExpr2(addExpr.left),
        parseExpr3(addExpr.right));
  }

  private static EasyScriptExprNode parseExpr3(EasyScriptParser.Expr3Context expr3) {
    if (expr3 instanceof EasyScriptParser.LiteralExpr3Context) {
      return parseLiteralExpr((EasyScriptParser.LiteralExpr3Context) expr3);
    } else if (expr3 instanceof EasyScriptParser.ReferenceExpr3Context) {
      return parseReferenceExpr((EasyScriptParser.ReferenceExpr3Context) expr3);
    } else {
      return parseExpr1(((EasyScriptParser.PrecedenceOneExpr3Context) expr3).expr1());
    }
  }

  private static EasyScriptExprNode parseLiteralExpr(EasyScriptParser.LiteralExpr3Context literalExpr) {
    TerminalNode intTerminal = literalExpr.literal().INT();
    if (intTerminal != null) {
      return parseIntLiteral(intTerminal.getText());
    }
    TerminalNode doubleTerminal = literalExpr.literal().DOUBLE();
    return doubleTerminal != null
        ? parseDoubleLiteral(doubleTerminal.getText())
        : new UndefinedLiteralExprNode();
  }

  private static GlobalVarReferenceExprNode parseReferenceExpr(EasyScriptParser.ReferenceExpr3Context refExpr) {
    String variableId = refExpr.ID().getText();
    return GlobalVarReferenceExprNodeGen.create(variableId);
  }

  private static EasyScriptExprNode parseIntLiteral(String text) {
    try {
      return new IntLiteralExprNode(Integer.parseInt(text));
    } catch (NumberFormatException e) {
      // 整数リテラルが大きすぎてJavaの32ビット `int` に収まらない可能性があります -
      // その場合は、doubleリテラルにフォールバックします
      return parseDoubleLiteral(text);
    }
  }

  private static DoubleLiteralExprNode parseDoubleLiteral(String text) {
    return new DoubleLiteralExprNode(Double.parseDouble(text));
  }
}
