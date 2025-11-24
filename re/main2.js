const s=c=>r=>t=>(a=>a(a)(t))(
  f=>p=>
    h(r)(p)?
      1:
      p.length!=0&&(p[0]==c||c==".")?
        f(f)(p.slice(1)):
        0)

const h=r=>t=>
  r.length==0?
    1:
    r[1]=="*"?
      s(r[0])(r.slice(2))(t):
      r[0]=="$"&&r.length==1?
        t.length==0:
        t.length!=0&&(r[0]=="."||r[0]==t[0])?
          h(r.slice(1))(t.slice(1)):
          0

const match=r=>t=>(a=>a(a)(t))(
  f=>p=>
    r[0]=="^"?
      h(r.slice(1))(p):
      h(r)(p)?
        1:
        p.slice(1).length>0?
          f(f)(p.slice(1)):
          0)

console.log(match("H...o")("Hello"))
console.log(match("a")("b"))
console.log(match("U.C")("UEC"))
console.log(match("U.C")("aEC"))
console.log(match("UU*EC")("UUUUEC"))
console.log(match("UU*C")("UEC"))


