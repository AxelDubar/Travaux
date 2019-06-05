import javafx.scene.canvas.GraphicsContext;

public class Droite {
	//Droite de la forme y=a*x+b
	double a,b;
	
	public Droite(Points p1, Points p2) {
		double xmilieu=(p1.getX()+p2.getX())/2;
		double ymilieu=(p1.getY()+p2.getY())/2;
		
		Points milieu=new Points(xmilieu, ymilieu);
		
		double pente=(p2.getY()-p1.getY())/(p2.getX()-p1.getX());
		a=(-1)*(1.0/pente);
		b=milieu.getY()-a*milieu.getX();
		
	}
	
	Points intersection(Droite d) {
		double x=(d.b-b)/(a-d.a);
		double y=a*x+b;
		Points interst=new Points(x,y);
		return interst;
	}
	
	public void draw(GraphicsContext context) {
		context.strokeLine(0, a*0+b, 790, a*790+b);
	}
	
}
