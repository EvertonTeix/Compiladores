package Java;

public class Cidade {
    private int x;
    private int y;

    public Cidade(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public double calcularDistancia(Cidade outraCidade) {
        int dx = this.x - outraCidade.x;
        int dy = this.y - outraCidade.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    @Override
    public String toString() {
        return String.format("Cidade (%d,%d)", x, y);
    }
}
