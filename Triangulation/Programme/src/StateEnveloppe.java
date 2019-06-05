import java.util.Set;

public class StateEnveloppe implements State {
	EnveloppeConvexe enveloppe;
	
	public StateEnveloppe(Set<Points> points) {
		enveloppe=new EnveloppeConvexe(points);
	}
	
	@Override
	public void mousePressed(DrawerContext context, double x, double y) {		
	}
	
	@Override
	public void draw(DrawerContext context) {
		enveloppe.draw(context.drawer.getGraphicsContext2D());
		
	}
}
