

import java.util.ArrayList;
import java.util.List;

import javafx.scene.canvas.GraphicsContext;

public class Triangulation {

	
	List<Triangle> triangles = new ArrayList<>();

	EnveloppeConvexe enveloppeConvexe;

	List<Points> pasEnveloppe;

	public Triangulation(EnveloppeConvexe enveloppeConvexe) {
		this.enveloppeConvexe = enveloppeConvexe;
		pasEnveloppe = enveloppeConvexe.pasEnveloppe();
		build();
	}


	void build() {
		List<Points> enveloppe=enveloppeConvexe.enveloppe();
		Points p1 = enveloppe.get(0);
		Points p2 = enveloppe.get(1);
		Points p3 = enveloppe.get(2);
		
		
		triangles.add(new Triangle(p1, p2, p3));

		for(int index = 2; index < enveloppe.size()-1; index++) {
			triangles.add(new Triangle(p1,enveloppe.get(index+1), enveloppe.get(index)));
		}
		if(!pasEnveloppe.isEmpty())	
			buildInnerTriangle();
		
	}

	void buildInnerTriangle() {
		for (Triangle tri : triangles) {
			if(tri.contains(pasEnveloppe.get(0))){
				
				triangles.add(0,new Triangle(tri.p1,tri.p2,pasEnveloppe.get(0)));
				triangles.add(0,new Triangle(tri.p1,tri.p3,pasEnveloppe.get(0)));
				triangles.add(0,new Triangle(tri.p3,tri.p2,pasEnveloppe.get(0)));
				pasEnveloppe.remove(0);
				if(pasEnveloppe.size() == 0)
					return;
				buildInnerTriangle();
				return;
			}
		}
	}

	void draw(GraphicsContext context) {
		for (Triangle triangle : triangles) {
			triangle.draw(context);
		}
	}


}
