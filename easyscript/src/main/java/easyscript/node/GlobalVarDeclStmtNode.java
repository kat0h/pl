package easyscript.node;

import easyscript.DeclarationKind;
import easyscript.EasyScriptException;
import easyscript.runtime.Undefined;

import com.oracle.truffle.api.dsl.NodeChild;
import com.oracle.truffle.api.dsl.NodeField;
import com.oracle.truffle.api.dsl.Specialization;

@NodeChild(value = "initializerExpr", type = EasyScriptExprNode.class)
@NodeField(name = "name", type = String.class)
@NodeField(name = "declarationKind", type = DeclarationKind.class)
public abstract class GlobalVarDeclStmtNode extends EasyScriptStmtNode {
  protected abstract String getName();
  protected abstract DeclarationKind getDeclarationKind();

  @Specialization
  protected Object createVariable(Object value) {
    String variableId = this.getName();
    boolean isConst = this.getDeclarationKind() == DeclarationKind.CONST;
    if (!this.currentLanguageContext().globalScopeObject.newVariable(variableId, value, isConst)) {
      throw new EasyScriptException(this, "Identifier '" + variableId + "' has already been declared");
    }
    return Undefined.INSTANCE;
  }
}
