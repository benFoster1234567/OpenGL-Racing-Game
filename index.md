---
layout: default
title: "My Game Engine Devlog"
---

# Welcome to my Game Engine Devlog!

Hi, I'm building a C++ motorcycle racing game using OpenGL. Follow my progress, engineering choices, and milestones below.

## Blog Posts
<ul>
  {% for post in site.posts %}
    <li>
      <a href="{{ post.url | relative_url }}">{{ post.title }}</a> — <i>{{ post.date | date: "%B %d, %Y" }}</i>
    </li>
  {% endfor %}
</ul>