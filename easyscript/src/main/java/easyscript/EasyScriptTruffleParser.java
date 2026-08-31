package easyscript;

import org.antlr.v4.runtime.BailErrorStrategy;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.TerminalNode;

import java.io.IOException;
import java.io.Reader;

public final class EasyScriptTruffleParser {
  public static EasyScriptNode parse(String program) {
    return parse(CharStreams.fromString(program));
  }
  public static EasyScriptNode parse(Reader program) throws IOException {
    return parse(CharStreams.fromReader(program));
  }
  public static EasyScriptNode parse(CharStream inputStream) {
    var lexer = new EasyScriptLexer(inputStream);
    lexer.removeErrorListeners();
    var parser = new EasyScriptParser(new CommonTokenStream(lexer));
    parser.removeErrorListeners();
    parser.setErrorHandler(new BailErrorStrategy());
    EasyScriptParser.ExprContext context = parser.start().expr();
    return parseExpr(context);
  }

  private static EasyScriptNode parseExpr(EasyScriptParser.ExprContext expr) {
    return expr instanceof EasyScriptParser.AddExprContext
      ? parseAdditionExpr((EasyScriptParser.AddExprContext) expr)
      : parseLiteralExpr((EasyScriptParser.LiteralExprContext) expr);
  }
  private static EasyScriptNode parseAdditionExpr(EasyScriptParser.AddExprContext addExpr) {
    return AdditionNodeGen.create(parseExpr(addExpr.left), parseExpr(addExpr.right));
  }
  private static EasyScriptNode parseLiteralExpr(EasyScriptParser.LiteralExprContext literalExpr) {
    TerminalNode intTerminal = literalExpr.literal().INT();
    // INTかDOUBLEかをチェック
    return intTerminal != null
      ? parseIntLiteral(intTerminal.getText())
      : parseDoubleLiteral(literalExpr.getText());
  }
  private static EasyScriptNode parseIntLiteral(String text) {
    try {
      return new IntLiteralNode(Integer.parseInt(text));
    } catch (NumberFormatException e) {
      return parseDoubleLiteral(text);
    }
  }
  private static EasyScriptNode parseDoubleLiteral(String text) {
    return new DoubleLiteralNode(Double.parseDouble(text));
  }
}
