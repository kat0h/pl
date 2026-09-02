package easyscript;

import easyscript.node.EasyScriptRootNode;
import easyscript.node.EasyScriptStmtNode;

import com.oracle.truffle.api.CallTarget;
import com.oracle.truffle.api.TruffleLanguage;
import java.util.List;


@TruffleLanguage.Registration(id = "ezs", name = "EasyScript")
public final class EasyScriptTruffleLanguage extends TruffleLanguage<EasyScriptLanguageContext> {
  @Override
  protected CallTarget parse(ParsingRequest request) throws Exception {
    List<EasyScriptStmtNode> stmts = EasyScriptTruffleParser.parse(request.getSource().getReader());
    var rootNode = new EasyScriptRootNode(this, stmts);
    return rootNode.getCallTarget();
  }
  @Override
  protected EasyScriptLanguageContext createContext(Env env) {
    return new EasyScriptLanguageContext();
  }
  @Override
  protected Object getScope(EasyScriptLanguageContext context) {
    return context.globalScopeObject;
  }
}
