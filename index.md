---
layout: default
title: "OpenGL Game Engine Devlog"
---

# My First Engineless Game 

Hi, I'm building a C++ motorcycle racing game using OpenGL. Follow my progress and milestones below.

## Blog Posts
<ul>
  {% for post in site.posts %}
    <li>
      <a href="{{ post.url | relative_url }}">{{ post.title }}</a> — <i>{{ post.date | date: "%B %d, %Y" }}</i>
    </li>
  {% endfor %}
</ul>