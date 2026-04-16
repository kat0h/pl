require_relative './main'
require_relative './main_ver2'
prgs = [[postfix, 0, 10, [swap, 2, mul, sub], 1, swap, exec],
        [postfix, 0, [5, [2, mul], exec], 3, swap],
        [postfix, 0, [[], exec], exec],
        [postfix, 0, 2, 3, 1, add, mul, sel],
        [postfix, 0, 2, 3, 1, [add], [mul], sel],
        [postfix, 0, 2, 3, 1, [add], [mul], sel, exec],
        [postfix, 0, 0, [2, 3, add], 4, sel, exec],
        [postfix, 0, 1, [2, 3, add], 4, sel, exec],
        [postfix, 0, [5, 6, lt], [2, 3, add], 4, sel, exec],
        [postfix, 0, [swap, exec, swap, exec], [1, sub], swap, [2, mul], swap, 3, swap, exec]]
prgs.each do |p|
  p eval_postfix(p, [])
  p eval_postfix_ver2(p, [])
end
