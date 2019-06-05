import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import javafx.scene.canvas.GraphicsContext;

public class TriangulationDelaunay {
	List<TriangleDelaunay> triangles;
	Set<Points> points;

	public TriangulationDelaunay(Set<Points> points) {
		this.points = points;
		triangles=hermeline(points);
	}
	
	List<TriangleDelaunay> hermeline(Set<Points> points){
		List<TriangleDelaunay> triangles=new ArrayList<>();
		List<Points> pointsConstruit=new ArrayList<>();
		/*Initialisation*/
		Points pointBasGauche=new Points(-1000000,-1000000);
		Points pointBasDroite=new Points(1002000,-1000000);
		Points pointHautGauche=new Points(-1000000,1002000);
		Points pointHautDroite=new Points(1002000,1002000);
		
		triangles.add(new TriangleDelaunay(pointBasGauche, pointBasDroite, pointHautGauche));
		pointsConstruit.add(pointBasGauche);
		pointsConstruit.add(pointBasDroite);
		pointsConstruit.add(pointHautGauche);
		triangles.add(new TriangleDelaunay(pointHautDroite,pointHautGauche,pointBasDroite));
		pointsConstruit.add(pointHautDroite);
		/*Construction*/
		
		for (Points point : points) {
			for (TriangleDelaunay triangle : triangles) {
				if(triangle.contains(point) && !triangle.estSommet(point)) {
					triangles.remove(triangle);
					TriangleDelaunay triangle1=new TriangleDelaunay(point, triangle.p1, triangle.p2);
					TriangleDelaunay triangle2=new TriangleDelaunay(point, triangle.p1, triangle.p3);
					TriangleDelaunay triangle3=new TriangleDelaunay(point, triangle.p2, triangle.p3);
					pointsConstruit.add(point);
					triangles.add(triangle1);
					triangles.add(triangle2);
					triangles.add(triangle3);
					if(!triangle1.estPlat())
						triangle1.valider(pointsConstruit, triangles);
					if(triangles.contains(triangle2) && !triangle2.estPlat())
							triangle2.valider(pointsConstruit, triangles);
					if(triangles.contains(triangle3) && !triangle3.estPlat())
						triangle3.valider(pointsConstruit, triangles);
					break;
				}
					
			}
		}
		for (int i=0;i<triangles.size();i++) {
			if(triangles.get(i).contains(pointBasGauche)||triangles.get(i).contains(pointBasDroite)||
					triangles.get(i).contains(pointHautDroite)||triangles.get(i).contains(pointHautGauche)) {
				triangles.remove(i);
				i--;
			}
		}
		
		return triangles;
	}
	
	void draw(GraphicsContext context) {
		for (TriangleDelaunay triangle : triangles) {
			triangle.draw(context);
		}
	}
	
}

