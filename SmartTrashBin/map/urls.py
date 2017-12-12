from django.conf.urls import url
from django.views.generic import TemplateView

urlpatterns = [
    url(r'^$', TemplateView.as_view(template_name="map/trash_bin_map.html"), name="map"),
    url(r'^state/(?P<pk>\d+)$', TemplateView.as_view(template_name="map/trash_bin_state.html")),
    url(r'^about$', TemplateView.as_view(template_name="map/about.html"), name="about"),
    url(r'^contact$', TemplateView.as_view(template_name="map/contact.html"), name="contact"),
]
