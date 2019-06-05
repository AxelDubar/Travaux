

import javafx.scene.canvas.GraphicsContext;

public class Triangle {
	
	Points p1;
	Points p2;
	Points p3;
	
	public Triangle(Points p1, Points p2, Points p3) {
		this.p1 = p1;
		this.p2 = p2;
		this.p3 = p3;
		
	}
	
	private boolean samePlan(Points a,Points b,Points c,Points p) {
		double xAB = b.getX() - a.getX();
		double xAC = c.getX() - a.getX();
		double xAP = p.getX() - a.getX();
		
		double yAB = b.getY() - a.getY();
		double yAC = c.getY() - a.getY();
		double yAP = p.getY() - a.getY();
		
		return (xAB*yAC - yAB*xAC) * (xAB*yAP - yAB*xAP) >= 0;
		
	}
	
	public boolean contains(Points p) {
		if(!samePlan(p1,p2,p3,p))
			return false;
		if(!samePlan(p1,p3,p2,p))
			return false;
		if(!samePlan(p3,p2,p1,p))
			return false;
		return true;
		
	}
	
	public void draw(GraphicsContext context) {
		context.strokeLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
		context.strokeLine(p2.getX(), p2.getY(), p3.getX(), p3.getY());
		context.strokeLine(p1.getX(), p1.getY(), p3.getX(), p3.getY());

	}
	
	public boolean estPlat() {
		double xP1P2 = p2.getX() - p1.getX();
		double xP1P3 = p3.getX() - p1.getX();
		
		double yP1P2 = p2.getY() - p1.getY();
		double yP1P3 = p3.getY() - p1.getY();
		
		return (xP1P2*yP1P3 - yP1P2*xP1P3) == 0;
	}
	
	public boolean estSommet(Points p) {
		return (p.getX()==p1.getX() && p.getY()==p1.getY()) || (p.getX()==p2.getX() && p.getY()==p2.getY()) || (p.getX()==p3.getX() && p.getY()==p3.getY());
	}

}
