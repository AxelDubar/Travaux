import javafx.scene.canvas.GraphicsContext;

public class Cercle {
	Points centre;
	double radius;
	
	public Cercle(Points centre, double radius) {
		this.centre = centre;
		this.radius = radius;
	}
	
	public boolean contains(Points p) {
		double dx=p.getX()-centre.getX();
		double dy=p.getY()-centre.getY();
		double distance=Math.sqrt(dx*dx+dy*dy);
		return distance<radius;
	}
	
	public void draw(GraphicsContext context) {
		context.strokeOval(centre.getX()-radius, centre.getY()-radius, radius*2, radius*2);
	}
}
