package aterm;

import aterm.visitor.*;

public class ATermVisitor
  implements Visitor
{
  //{{{ public void visit(Visitable visitable) throws ATermVisitFailure

  public void visit(Visitable visitable) throws ATermVisitFailure
  {
    ((ATermVisitable)visitable).accept(this);
  }

  //}}}

  //{{{ public void visitATerm(ATerm arg) throws ATermVisitFailure

  public void visitATerm(ATerm arg) throws ATermVisitFailure
  {
  }

  //}}}

  //{{{ public void visitInt(ATermInt arg) throws ATermVisitFailure

  public void visitInt(ATermInt arg) throws ATermVisitFailure
  {
    visitATerm(arg);
  }

  //}}}
  //{{{ public void visitReal(ATermReal arg) throws ATermVisitFailure

  public void visitReal(ATermReal arg) throws ATermVisitFailure
  {
    visitATerm(arg);
  }

  //}}}
  //{{{ public void visitAppl(ATermAppl arg) throws ATermVisitFailure

  public void visitAppl(ATermAppl arg) throws ATermVisitFailure
  {
    visitATerm(arg);
  }

  //}}}
  //{{{ public void visitList(ATermList arg) throws ATermVisitFailure

  public void visitList(ATermList arg) throws ATermVisitFailure
  {
    visitATerm(arg);
  }

  //}}}
  //{{{ public void visitPlaceholder(ATermPlaceholder arg) throws ATermVisitFailure

  public void visitPlaceholder(ATermPlaceholder arg) throws ATermVisitFailure
  {
    visitATerm(arg);
  }

  //}}}
  //{{{ public void visitBlob(ATermBlob arg) throws ATermVisitFailure

  public void visitBlob(ATermBlob arg) throws ATermVisitFailure
  {
    visitATerm(arg);
  }

  //}}}
}
