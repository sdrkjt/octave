package aterm;

import aterm.visitor.*;

public class ATermForward
  extends ATermVisitor
{
  private Visitor visitor;

  //{{{ public ATermForward(Visitor visitor)

  public ATermForward(Visitor visitor)
  {
    this.visitor = visitor;
  }

  //}}}
  //{{{ public void visit(Visitable visitable)

  public void visit(Visitable visitable)
    throws ATermVisitFailure
  {
    try {
      visitor.visit(visitable);
    } catch (VisitFailure ex) {
      ATermVisitFailure vf = new ATermVisitFailure(ex.getMessage());
      vf.fillInStackTrace();
      throw vf;
    }
  }

  //}}}
}
