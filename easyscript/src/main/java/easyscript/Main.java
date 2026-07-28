package easyscript;

public class Main {
  public static void main(String[] args) {
    // part1のまま
    System.out.println("Hello");
    EasyScriptNode n = new IntLiteralNode(123);
    var rootNode = new EasyScriptRootNode(n);
    System.out.println("execute: " + rootNode.getCallTarget().call());
  }
}
