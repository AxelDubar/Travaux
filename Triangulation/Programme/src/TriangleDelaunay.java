import java.util.ArrayList;
import java.util.List;

import javafx.scene.canvas.GraphicsContext;

public class TriangleDelaunay extends Triangle{

	Cercle cercle;

	public TriangleDelaunay(Points p1, Points p2, Points p3) {
		super(p1, p2, p3);
		if(p1.getY()!=p2.getY() && p3.getY()!=p2.getY()) {
			Droite d1=new Droite(p1,p2);
			Droite d2=new Droite(p2,p3);
			Points centreC=d1.intersection(d2);
	
			double xCP=p1.getX()-centreC.getX();
			double yCP=p1.getY()-centreC.getY();
			double radius=Math.sqrt(xCP*xCP+yCP*yCP);
			cercle=new Cercle(d1.intersection(d2),radius );
		}
		else if(p1.getY()==p2.getY() && p3.getY()!=p2.getY()) {
			Droite d1=new Droite(p1,p3);
			Droite d2=new Droite(p2,p3);
			Points centreC=d1.intersection(d2);
	
			double xCP=p1.getX()-centreC.getX();
			double yCP=p1.getY()-centreC.getY();
			double radius=Math.sqrt(xCP*xCP+yCP*yCP);
			cercle=new Cercle(d1.intersection(d2),radius );
		}
		else if(p3.getY()==p2.getY() && p1.getY()!=p2.getY()){
			Droite d1=new Droite(p1,p3);
			Droite d2=new Droite(p2,p1);
			Points centreC=d1.intersection(d2);
	
			double xCP=p1.getX()-centreC.getX();
			double yCP=p1.getY()-centreC.getY();
			double radius=Math.sqrt(xCP*xCP+yCP*yCP);
			cercle=new Cercle(d1.intersection(d2),radius );
		}
			
	}

	public void valider(List<Points> points, List<TriangleDelaunay> triangles) {
		for (Points point : points) {
			if(cercle.contains(point)) {//Boule non vide
				if(estSommet(point))
					continue;
				
				
				TriangleDelaunay triangleproblematique=null;
				for (TriangleDelaunay triangle : adjacent(triangles)) {
					if(triangle.estSommet(point)) {
						triangleproblematique=triangle;
						break;						
					}
				}
				if(triangleproblematique==null)
						continue;
				triangles.remove(this);
				triangles.remove(triangleproblematique);
				TriangleDelaunay triangleCorrige1 = null,triangleCorrige2 = null;
				if(!triangleproblematique.estSommet(p1)) {
					triangleCorrige1=new TriangleDelaunay(point, p1, p2);
					triangleCorrige2=new TriangleDelaunay(point, p1, p3);
				}
				if(!triangleproblematique.estSommet(p2)) {
					triangleCorrige1=new TriangleDelaunay(point, p2, p3);
					triangleCorrige2=new TriangleDelaunay(point, p2, p1);
				}
				if(!triangleproblematique.estSommet(p3)) {
					triangleCorrige1=new TriangleDelaunay(point, p3, p2);
					triangleCorrige2=new TriangleDelaunay(point, p3, p1);
				}
				triangles.add(triangleCorrige1);
				triangles.add(triangleCorrige2);
				if(!triangleCorrige1.estPlat())
					triangleCorrige1.valider(points, triangles);
				
				if(triangles.contains(triangleCorrige2)&& !triangleCorrige2.estPlat())
					triangleCorrige2.valider(points, triangles);
				return;
			}
		}
		
	}

	public List<TriangleDelaunay> adjacent(List<TriangleDelaunay> triangles){
		List<TriangleDelaunay> listAdjacent=new ArrayList<>();
		for (TriangleDelaunay triangle : triangles) {
			if(estAdjacent(triangle))
				listAdjacent.add(triangle);
		}
		
		return listAdjacent;
	}
	
	public boolean estAdjacent(TriangleDelaunay triangle) {
		int cpt=0;
		if(estSommet(triangle.p1)) {
			cpt++;
		}
		if(estSommet(triangle.p2)) {
			cpt++;
		}
		if(estSommet(triangle.p3)) {
			cpt++;
		}
		return cpt==2;
	}
	public void draw(GraphicsContext context) {
		context.strokeLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
		context.strokeLine(p2.getX(), p2.getY(), p3.getX(), p3.getY());
		context.strokeLine(p1.getX(), p1.getY(), p3.getX(), p3.getY());
		//cercle.draw(context);
		
	}
}
