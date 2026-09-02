package easyscript;

public enum DeclarationKind {
  VAR, LET, CONST;
  public static DeclarationKind fromToken(String token) {
    switch (token) {
      case "var": return DeclarationKind.VAR;
      case "let": return DeclarationKind.LET;
      case "const": return DeclarationKind.CONST;
      default: throw new EasyScriptException("Unrecognized variable kind: '" + token + "'");
    }
  }
}

