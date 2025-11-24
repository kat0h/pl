function match(regex,text){
  if(regex[0]=="^")
    return matchhere(regex.slice(1),text);
  do{
    if(matchhere(regex,text))return 1;
  }while(text=text.slice(1),text.length>0);
  return 0;
}

function matchhere(regex,text){
  console.log(`here regex:${regex}\ttext:${text}`)
  if(regex.length==0)return 1;
  if(regex[1]=="*")return matchstar(regex[0],regex.slice(2),text);
  if(regex[0]=="$"&&regex.length==1)return text.length==0;
  if(text.length!=0&&(regex[0]=="."||regex[0]==text[0]))return matchhere(regex.slice(1),text.slice(1));
  return 0;
}

function matchstar(c,regex,text){
  console.log(`star regex:${regex}\ttext:${text}\tc:${c}`)
  do{
    if(matchhere(regex,text))return 1;
  }while(text.length!=0&&((t=text[0],text=text.slice(1),t)==c||c=="."));
  return 0;
}

console.log(match("H...o","Hello"))
console.log(match("a","b"))
console.log(match("U.C","UEC"))
console.log(match("U.C","aEC"))
console.log(match("UU*EC","UUUUEC"))
console.log(match("UU*C","EC"))
