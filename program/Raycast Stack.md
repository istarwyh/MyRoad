#technology 

[Our stack](https://www.raycast.com/jobs/software-engineer-macos)
Swift for our macOS app. Everything is written in Swift using AppKit with Swift Packages. We don't use SwiftUI, however we are likely to start using it soon. We rarely adopt third party dependencies. Some are GRDB for our database, Sentry for crash reporting, Nuke for image fetching and a few C libraries to crank up performance.

React + Node.js for Extensions API. We use Node.js as a runtime and React as a UI framework for extensions. React components are rendered as native AppKit components, no HTML or CSS involved. The entire API is written in TypeScript. The setup is very similar to how React Native works on mobile.

JavaScript/TypeScript for marketing website. We use Next.js for raycast.com and deploy it with Vercel.

Ruby on Rails for backend. We use Ruby on Rails for our Extension Store backend and Heroku for hosting/deployment.

Modern work tools for everything. Linear for issue tracking, GitHub as source control, Sentry for error reporting, Segment and Amplitude for analytics, Slack for internal communication, Figma for pixels, Notion as knowledge base and a few others. As a team, we enjoy using CleanShot, Bear, Things and a few other Mac apps.