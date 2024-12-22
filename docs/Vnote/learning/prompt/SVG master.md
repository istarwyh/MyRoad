## CONTEXT
我需要为一篇文章生成SVG装饰元素，包括标题、章节标题和装饰性元素。这些SVG需要在移动设备上也能良好显示。

输入文章信息：
- 标题：[文章标题]
- 章节数：[章节数量]
- 风格倾向：[现代/传统/简约等]
- 情感基调：[严肃/轻松/忧郁等]
- 目标设备：[PC/移动端/两者]

## OBJECTIVE
为文章生成以下SVG元素：
1. 响应式样式定义
2. 标题装饰
3. 章节标题装饰
4. 引用框装饰
5. 结尾装饰

## STEPS
1. 样式分析
   - 分析文章风格和情感基调
   - 确定配色方案
   - 设计装饰元素风格

2. 响应式规划
   - 设计viewBox尺寸
   - 规划preserveAspectRatio策略
   - 定义CSS类

3. 元素设计
   - 标题布局和装饰
   - 章节标题样式
   - 装饰性图形选择
   - 文字排版规划

4. 移动适配
   - 简化复杂图形
   - 调整字体大小
   - 优化线条粗细
   - 确保清晰可读

5. 性能优化
   - 简化路径数据
   - 复用共同元素
   - 优化动画效果
   - 减少不必要的嵌套

## SPECIFICATIONS
### 样式规范
```css
.responsive-svg {
    width: 100%;
    height: auto;
    max-width: 600px;
    margin: 0 auto;
    display: block;
}
```
### SVG基础模板

```
<svg class="responsive-svg" viewBox="0 0 300 80" 
     xmlns="http://www.w3.org/2000/svg" 
     preserveAspectRatio="xMidYMid meet">
    <!-- 背景 -->
    <!-- 装饰元素 -->
    <!-- 文字内容 -->
</svg>
```

### 配色方案
主色：#d4a373（暖色调）
背景：#fff8f0（柔和背景）
文字：#774936（深色文字）
装饰：#adb5bd（中性灰）

### TONE
专业性：保持设计的专业性和一致性
优雅性：确保装饰元素优雅且不喧宾夺主
适应性：考虑不同设备的显示效果

### AUDIENCE
- 文章读者
- 移动端用户
- 设计开发人员

### EXAMPLES
#### 标题SVG示例

```
<svg class="responsive-svg" viewBox="0 0 300 80">
    <rect width="100%" height="100%" fill="#fff8f0"/>
    <circle cx="30" cy="40" r="15" fill="none" 
            stroke="#d4a373" stroke-width="1.5"/>
    <text x="150" y="45" font-family="楷体" 
          font-size="18" fill="#774936" 
          text-anchor="middle">
        [标题文本]
    </text>
</svg>
```
### VALIDATION CHECKLIST
[ ] 响应式设计已实现
[ ] 移动端显示正常
[ ] 字体大小合适
[ ] 装饰元素简洁优雅
[ ] 性能已优化
[ ] 代码规范整洁

## RESPONSIVE DESIGN
### Breakpoint Strategy
```css
/* Base styles for mobile */
.svg-container {
  width: 100%;
  max-width: 600px;
}

/* Tablet breakpoint */
@media (min-width: 768px) {
  .svg-container {
    max-width: 720px;
  }
}

/* Desktop breakpoint */
@media (min-width: 1024px) {
  .svg-container {
    max-width: 960px;
  }
}
```

### Touch Targets
- Minimum touch target size: 44x44px
- Adequate spacing between interactive elements
- Clear hover/focus states
- Touch feedback animations

### Device-Specific Optimizations
- Reduce complexity on mobile devices
- Adjust stroke widths for different pixel densities
- Use appropriate text sizes for each breakpoint
- Consider device orientation changes

## FRAMEWORK INTEGRATION
### React Integration
```jsx
const SVGComponent = ({ title, description, ...props }) => (
  <svg
    role="img"
    aria-labelledby="title desc"
    {...props}
  >
    <title id="title">{title}</title>
    <desc id="desc">{description}</desc>
    {/* SVG content */}
  </svg>
);
```

### Vue Integration
```vue
<template>
  <svg
    :class="['svg-container', responsiveClass]"
    :aria-label="description"
    role="img"
  >
    <!-- SVG content -->
  </svg>
</template>

<script>
export default {
  props: {
    description: String,
    responsiveClass: String
  }
}
</script>
```

### Dynamic SVG Generation
- Server-side rendering considerations
- Client-side manipulation guidelines
- State management integration
- Event handling patterns

## ACCESSIBILITY GUIDELINES
### ARIA Integration
- Add role="img" for decorative SVGs
- Include aria-label for meaningful graphics
- Provide aria-hidden="true" for purely decorative elements
- Ensure meaningful title and desc elements

### Color and Contrast
- Maintain WCAG 2.1 AA contrast ratio (4.5:1 minimum)
- Provide alternative styles for color-blind users
- Include high-contrast mode support
- Test with color blindness simulators

### Screen Reader Support
- Implement meaningful text alternatives
- Structure content hierarchically
- Test with popular screen readers (NVDA, VoiceOver)

## TECHNICAL SPECIFICATIONS
### Optimization Requirements
- Maximum file size: 10KB for simple SVGs, 30KB for complex
- Use SVGO for optimization with custom config:
```json
{
  "plugins": [
    "removeViewBox": false,
    "cleanupIDs": true,
    "removeEmptyAttrs": true
  ]
}
```

### Browser Support
- Target browsers: Latest 2 versions of major browsers
- Fallback strategy:
  ```html
  <picture>
    <source srcset="image.svg" type="image/svg+xml">
    <img src="fallback.png" alt="Description">
  </picture>
  ```

### Build Integration
- Webpack/Vite configuration for SVG handling
- Sprite sheet generation setup
- Source map generation for debugging

## PERFORMANCE OPTIMIZATION
### File Size Guidelines
- Implement lazy loading for off-screen SVGs
- Use CSS sprites for recurring icons
- Compress paths using relative commands
- Remove unnecessary metadata

### Animation Performance
- Use CSS transforms over path manipulation
- Implement requestAnimationFrame for smooth animations
- Provide reduced motion alternatives
- Monitor frame rate impact

### Caching Strategy
- Implement cache-control headers
- Version SVG assets in production
- Use service workers for offline support
```html
<svg>
  <defs>
    <style>
      @media (prefers-reduced-motion: reduce) {
        .animated-element {
          animation: none;
        }
      }
    </style>
  </defs>
</svg>
```

## TESTING AND VALIDATION
### Unit Testing
```javascript
describe('SVGComponent', () => {
  it('should render with proper accessibility attributes', () => {
    const { getByRole } = render(
      <SVGComponent title="Test" description="Test desc" />
    );
    const svg = getByRole('img');
    expect(svg).toHaveAttribute('aria-labelledby');
  });
});
```

### Browser Testing Matrix
| Browser | Version | Support Level |
|---------|---------|---------------|
| Chrome  | Latest 2| Full          |
| Firefox | Latest 2| Full          |
| Safari  | Latest 2| Full          |
| Edge    | Latest 2| Full          |
| IE11    | -       | Fallback      |

### Visual Regression Testing
- Implement screenshot comparison tests
- Test across different viewport sizes
- Validate animations and transitions
- Check rendering in different color modes

## SECURITY GUIDELINES
### XSS Prevention
```javascript
// Sanitize dynamic content
const sanitizeSVG = (content) => {
  const config = {
    allowedTags: ['svg', 'path', 'circle', 'rect'],
    allowedAttributes: {
      'svg': ['viewBox', 'xmlns'],
      '*': ['fill', 'stroke']
    }
  };
  return DOMPurify.sanitize(content, config);
};
```

### Content Security Policy
```html
<meta http-equiv="Content-Security-Policy" 
      content="default-src 'self'; img-src 'self' data: https:;">
```

### User Input Handling
- Validate all user inputs before rendering
- Escape special characters
- Implement rate limiting for dynamic SVGs
- Use secure random IDs for elements

## MAINTAINABILITY
### Naming Conventions
```css
/* Component-specific prefixes */
.svg-header__title {}
.svg-header__decoration {}

/* State modifiers */
.svg-icon--active {}
.svg-icon--disabled {}

/* Utility classes */
.svg-u-responsive {}
.svg-u-hidden {}
```

### Documentation Requirements
- Component API documentation
- Usage examples
- Performance considerations
- Accessibility requirements
- Browser support notes

### Version Control
- Semantic versioning (MAJOR.MINOR.PATCH)
- Changelog maintenance
- Git hooks for validation
- Asset versioning strategy

### Code Review Checklist
- [ ] Accessibility compliance
- [ ] Performance metrics met
- [ ] Security measures implemented
- [ ] Documentation updated
- [ ] Tests added/updated
- [ ] Browser compatibility verified