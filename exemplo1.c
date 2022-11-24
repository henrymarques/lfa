int f()
{
  a = 10;

  b = 3;
  return b;
}
float g()
{
   a = 10;
   b = 3;
   c = (a+b)*2;
   a = a + 1 + (b + (c + 1) * d);
   b = b*f()+10;
   a = 10;
   return c;
}
