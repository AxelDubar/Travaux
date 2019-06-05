import java.util.Scanner;
import java.util.Set;
import java.util.TreeSet;
import javafx.scene.input.*;

public class DrawerContext {
	Drawer drawer;
	State currentState;

	Set<Points> points = new TreeSet<>(); 
	
	public DrawerContext(Drawer drawer) {
		this.drawer = drawer;
		currentState=new StatePoint();
	}
	
	
	void mousePressed(MouseEvent event){
        drawer.repaint();
        double x = event.getX();
        double y = event.getY();
        draw();
        currentState.mousePressed(this,x,y);

    }
	
	void keyPressed(javafx.scene.input.KeyEvent event) {
		
		switch (event.getText()) {
			case "h":
				if(currentState instanceof StatePoint) {
					int n=0;
					@SuppressWarnings("resource")
					Scanner sc=new Scanner(System.in);
					System.out.println("Entrer le nombre de points voulu:");
					n=sc.nextInt();
					for(int i=0;i<n;i++) {
						int x = (int) (Math.random() * 700) +50;
						int y = (int) (Math.random() * 700) +50;
						points.add(new Points(x,y));
					}
					draw();
				}
				else
					System.out.println("Entrer en editeur de point pour ajouter des points (touche p)");
				
				break;
	        case "t":
	            if(points.size()>2) {
	                currentState=new StateTriangulation(points);
	                draw();
	            }
	            break;
	            
	        case "d":
	        	if(points.size()>2) {
	        		currentState=new StateDelaunay(points);
	        		draw();
	        	}
	        	break;
	        case "p":
	        	currentState=new StatePoint();
	        	draw();
	        	break;
	        case "c":
	        	currentState=new StatePoint();
	        	points.clear();
	        	draw();
	        	break;
	        case "e":
	        	currentState=new StateEnveloppe(points);
	        	draw();
	        	break;
	        case "s":
	        	currentState=new StateSuppression();
	        	draw();
	        	break;
		}
	}
	
	void draw() {
		drawer.repaint();
		for (Points point : points) {
			drawer.getGraphicsContext2D().fillOval(point.getX()-3, point.getY()-3, 6, 6);
		}
		currentState.draw(this);
	}
}
	
