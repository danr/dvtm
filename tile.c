
static void tile(void)
{
	//unsigned int i, n, nx, ny, nw, nh, m, mw, mh, th;
	Client *c;

	unsigned int slack, used, sections, xo;
	unsigned int n[T];
	unsigned int i[T];
	unsigned int h0[T];
	unsigned int h[T];
	unsigned int w[T];
	unsigned int x[T];
	unsigned int y[T];
	unsigned int t;

	float m;

	for (t = 0; t < T; ++t) {
		n[t] = 0;
	}

	for (c = nextvisible(clients); c; c = nextvisible(c->next)) {
		if (!c->minimized) {
			n[bin(c->tags)]++;
		}
	}

	sections = 0;
	m = 0.0;
	for (t = 0; t < T; ++t) {
		sections += n[t] > 0;
		if (n[t] > 0) {
			m += screen.mfact[t];
		}
	}

	if (sections == 0) {
		// no visible windows
		return;
	}

	used = 0;
	for (t = 0; t < T; ++t) {
		w[t] = n[t] ? (int)((float)(waw - sections + 1) * screen.mfact[t] / m) - 1 : 0;
		used += w[t];
	}

	slack = waw - sections + 1 - used;

	// distribute the slack
	t = slack > 0;
	while (slack != 0) {
		if (n[t] && slack > 0) {
			w[t]++;
			slack--;
		} else if (n[t] && w[t] > 0 && slack < 0) {
			w[t]--;
			slack++;
		}
		t++;
		if (t > T) {
			t = 0;
		}
	}

	xo = 0;

	for (t = 0; t < T; ++t) {
		if (n[t]) {
			x[t] = xo;
			xo += w[t] + 1;
			if (xo - 1 < waw) {
				mvvline(way, xo - 1, ACS_VLINE, wah);
			}
		}
	}

	for (t = 0; t < T; ++t) {
		if (n[t]) {
			h[t] = wah / n[t];
			h0[t] = wah - h[t] * (n[t] - 1);
			y[t] = 0;
			i[t] = 0;
		}
	}

	for (c = nextvisible(clients); c; c = nextvisible(c->next)) {
		if (c->minimized)
			continue;
		t = bin(c->tags);
		resize(c, x[t], way + h[t] * i[t], w[t], h[t]);
		i[t]++;
	}
}
