
public class StatePoint implements State {

	@Override
	public void mousePressed(DrawerContext context, double x, double y) {
		Points p=new Points((int)x,(int)y);
		Cercle cercle=new Cercle(p, 6);
		for (Points point : context.points) {
			if(cercle.contains(point)) {
				System.out.println("Il y a deja un point");
				return;
			}
		}
		context.points.add(p);
		context.draw();
	}

	@Override
	public void draw(DrawerContext context) {
		
	}
}
