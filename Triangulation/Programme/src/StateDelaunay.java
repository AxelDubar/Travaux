import java.util.Set;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class StateDelaunay implements State{
	TriangulationDelaunay triangulation;
	
	public StateDelaunay(Set<Points> points) {
		triangulation=new TriangulationDelaunay(points);
		
	}
	
	@Override
	public void mousePressed(DrawerContext context, double x, double y) {
		Points p=new Points((int)x,(int)y);
		GraphicsContext graphic=context.drawer.getGraphicsContext2D();
		for (TriangleDelaunay triangle : triangulation.triangles) {
			if(triangle.contains(p)) {
				graphic.setStroke(new Color(1,0,0,1));
				triangle.draw(graphic);
				triangle.cercle.draw(graphic);
				graphic.setStroke(new Color(0,0,0,1));
			}
				
		}
	}
	
	@Override
	public void draw(DrawerContext context) {
		triangulation.draw(context.drawer.getGraphicsContext2D());
		
	}
}
