(isstr (strftime ("%%%n%t%H%I%k%l", localtime (time ())))
 && isstr (strftime ("%M%p%r%R%s%S%T", localtime (time ())))
 && isstr (strftime ("%X%Z%z%a%A%b%B", localtime (time ())))
 && isstr (strftime ("%c%C%d%e%D%h%j", localtime (time ())))
 && isstr (strftime ("%m%U%w%W%x%y%Y", localtime (time ()))))
