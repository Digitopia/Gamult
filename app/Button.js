function Button (module, buttonType, title) {
	this.module = module;
	this.type = type;
	this.title = title;
	this.state = false;
	this.id = -1;

	this.setDimensions = function(x, y, size) {
		this.x = x;
		this.y = y;
	}
}