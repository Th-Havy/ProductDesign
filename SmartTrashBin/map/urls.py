from django.conf.urls import url
from django.views.generic import TemplateView

urlpatterns = [
    url(r'^$', TemplateView.as_view(template_name="map/trash_bin_map.html")),
    url(r'^state/$', TemplateView.as_view(template_name="map/trash_bin_state.html")),
]
