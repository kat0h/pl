package easyscript;

import com.oracle.truffle.api.CallTarget;
import com.oracle.truffle.api.TruffleLanguage;
import com.oracle.truffle.api.TruffleLanguage.Env;
import com.oracle.truffle.api.TruffleLanguage.ParsingRequest;

@TruffleLanguage.Registration(id = "ezs", name = "EasyScript")
public final class EasyScriptTruffleLanguage extends TruffleLanguage<Void> {
  @Override
  protected CallTarget parse(ParsingRequest request) throws Exception {
    EasyScriptNode exprNode = EasyScriptTruffleParser.parse(request.getSource().getReader());
    var rootNode = new EasyScriptRootNode(exprNode);
    return rootNode.getCallTarget();
  }
  @Override
  protected Void createContext(Env env) {
    return null;
  }
}
