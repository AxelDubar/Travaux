import java.util.Set;

public class StateTriangulation implements State {

	Triangulation triangulation;
	
	public StateTriangulation(Set<Points> points) {
		EnveloppeConvexe enveloppe=new EnveloppeConvexe(points);
		triangulation=new Triangulation(enveloppe);
	}
	
	@Override
	public void mousePressed(DrawerContext context, double x, double y) {
		
	}
	
	@Override
	public void draw(DrawerContext context) {
		triangulation.draw(context.drawer.getGraphicsContext2D());
	
	}
	
}
