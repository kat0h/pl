package easyscript;

public class Main {
  public static void main(String[] args) {
    System.out.println("Hello");
    EasyScriptNode n = new IntLiteralNode(123);
    System.out.println("execute: " + n.executeInt(null));
  }
}
